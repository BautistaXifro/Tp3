#include "client_manager.h"
#include <utility>
#include <arpa/inet.h>
#include <iostream>
#include <atomic>
#define MAX_BUFFER_LENGTH 256
#define PLAY 112
#define CREATE 110
#define LIST 108
#define JOIN 106

int static get_length(char* length);
void static set_to_send_length(char* buffer, const short& length);

ClientManager::ClientManager(Socket socket, ProtectedMap& m) : 
    socket_client(std::move(socket)), map(m){
        this->finish = false;
    }

void ClientManager::run(){
    while(!this->finish){
        char firstByte[1];
        this->socket_client.receive(firstByte, 1);
        union{
            short number;
            char buffer[2];
        }byte;
        byte.number = 0;
        byte.buffer[1] = firstByte[0];
        int client_command = ntohs(byte.number);
        switch(client_command){
            case PLAY:
                play();
                break;
            case CREATE:
                create();
                break;
            case LIST:
                list();
                break;
            case JOIN:
                join_game();
                break;
            default:
                std::cerr << "Error en el comando de cliente\n";
        }
    }
}

void ClientManager::create(){
    char game_name[MAX_BUFFER_LENGTH];
    char length[2];
    this->socket_client.receive(length, 2);
    int name_length = get_length(length);

    this->socket_client.receive(game_name, name_length);
    std::string game(game_name);
    this->map.insert(game);
    this->map.find(game, this->board);
    this->simbol = 'O';
    print_board();
}

void ClientManager::list(){
    std::string allGames("  Partidas:");
    std::string separator("\n - ");
    this->map.get_keys(allGames, separator);
    char* buffer = new char[sizeof(allGames) + 1];
    //aloco en el heap ya que el size depende de la cantidad
    //de partidas que haya en juego por lo tanto no tengo un maximo posible
    short length = allGames.copy(buffer, allGames.size(), 0);
    buffer[length] = '\n';

    set_to_send_length(buffer, length - 1);// menos el byte de longitud

    this->socket_client.send(buffer, length + 1);
    delete[] buffer;
}

void ClientManager::join_game(){
    char game_name[MAX_BUFFER_LENGTH];
    char length[2];
    this->socket_client.receive(length, 2);
    int name_length = get_length(length);

    this->socket_client.receive(game_name, name_length);
    std::string game(game_name);

    this->map.find(game, this->board);
    this->simbol = 'X';
    print_board();
}

void ClientManager::play(){
    char buffer[2];
    this->socket_client.receive(buffer, 2);
    int column = buffer[0] - 48;
    int row = buffer[1] - 48;
    this->board->put(column, row, this->simbol);
    print_board();
}

void ClientManager::print_board(){
    char buffer[MAX_BUFFER_LENGTH];
    std::string string_board("  ");
    if (this->board->print(string_board, this->simbol) == 1){
        this->finish = true;
    }
    short board_length = string_board.copy(buffer, string_board.size(), 0);
    
    //le resto los bytes de longitud
    set_to_send_length(buffer, board_length - 2);
    this->socket_client.send(buffer, board_length);
}

int static get_length(char* length){
    union{
        short number;
        char buffer[2];
    }byte;
    byte.number = 0;

    byte.buffer[0] = length[0];
    byte.buffer[1] = length[1];
    return ntohs(byte.number);
}

void static set_to_send_length(char* buffer, const short& length){
    union{
        short number;
        char buffer[2];
    }byte;

    byte.number = htons(length);
    buffer[0] = byte.buffer[0];
    buffer[1] = byte.buffer[1];
}

bool ClientManager::gameFinish(){
    return this->finish.load();
}

ClientManager::~ClientManager(){
    this->socket_client.close();
}