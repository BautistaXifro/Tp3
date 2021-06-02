#include "client_manager.h"
#include <utility>
#include <arpa/inet.h>
#include <iostream>
#include <atomic>
#include <string>

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
        this->simbol = ' ';
        this->queue = nullptr;
    }

void ClientManager::run(){
    while (!this->finish){
        char firstByte[1];
        this->socket_client.receive(firstByte, 1);
        union{
            short number;
            char buffer[2];
        }byte;
        byte.number = 0;
        byte.buffer[1] = firstByte[0];
        int client_command = ntohs(byte.number);
        switch (client_command){
            case PLAY:
                play();
                print_board();
                break;
            case CREATE:
                create();
                print_board();
                break;
            case LIST:
                list();
                break;
            case JOIN:
                join_game();
                print_board();
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
    this->map.find(game, this->queue);
    this->simbol = 'O';
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

    this->map.find(game, this->queue);
    this->simbol = 'X';
    
    std::vector<int> values = this->queue->pop();
    this->board.put(values[0], values[1], values[2]);
}

void ClientManager::play(){
    char buffer[1];
    this->socket_client.receive(buffer, 1);
    uint8_t value = buffer[0];
    int column = (int) value >> 4;
    int row = (int) value & 15;
    int aux[] = {column, row, this->simbol};
    std::vector<int> values(aux, aux + 3);
    this->queue->push(values);
    if (this->board.put(column, row, this->simbol) == 1) return;
    std::vector<int> otherPlayerValues = this->queue->pop();
    this->board.put(otherPlayerValues[0], 
            otherPlayerValues[1], otherPlayerValues[2]);
}

void ClientManager::print_board(){
    char buffer[MAX_BUFFER_LENGTH];
    std::string string_board("  ");
    if (this->board.print(string_board, this->simbol) == 1){
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
