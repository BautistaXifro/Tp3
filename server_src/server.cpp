#include "server.h"
#include "client_manager.h"
#include "board.h"
#include <list>
#include <arpa/inet.h>
#include <iostream>
#define MAX_BUFFER_LENGTH 256
#define PLAY 112
#define CREATE 110
#define LIST 108
#define JOIN 106

int static get_length(char* length);
void static set_to_send_length(char* buffer, const short& length);

Server::Server(const char* port){
    this->socket_server.bind(port);
    this->socket_client = std::move(this->socket_server.accept());
}

void Server::run(){
    //la condicion no la pense todavia
    /*while (true){
        this->clients.push_back(new ClientManager(std::move(this->socket_server.accept())));
        Concepto de como manejar lo clientes con threads
    }*/
    int i = 0;
    while(i != 2){
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
        i++;
    }
}

void Server::create(){
    char game_name[MAX_BUFFER_LENGTH];
    char length[2];
    this->socket_client.receive(length, 2);
    int name_length = get_length(length);

    this->socket_client.receive(game_name, name_length);
    std::string game(game_name);
    this->map.insert(game);
    print_board(game);
}

void Server::list(){
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

void Server::join_game(){
    char game_name[MAX_BUFFER_LENGTH];
    char length[2];
    this->socket_client.receive(length, 2);
    int name_length = get_length(length);

    this->socket_client.receive(game_name, name_length);
    std::string game(game_name);
    // y antes de que busque necesito esperar que juegue el primer player
    // capaz que con un condition variable
    find_and_print_board(game);
}

void Server::find_and_print_board(const std::string& game){
    Board* board_reference;
    this->map.find(game, board_reference);
    char buffer[MAX_BUFFER_LENGTH];
    std::string board("  ");
    board_reference->print(board);

    //le resto los bytes de longitud
    short board_length = board.copy(buffer, board.size(), 0) - 2;
    
    set_to_send_length(buffer, board_length);
    this->socket_client.send(buffer, board_length + 2);
}

void Server::play(){
    
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

//capaz que tiene que ir en una funcion y no en el destructor
Server::~Server(){
    /*for(auto client : clients){
        client.close();
    }*/
    this->socket_client.close();
    this->socket_server.close();
}
