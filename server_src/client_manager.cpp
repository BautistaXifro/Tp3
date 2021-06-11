#include "client_manager.h"
#include <utility>
#include <arpa/inet.h>
#include <iostream>
#include <atomic>
#include <string>
#include <vector>

#define MAX_BUFFER_LENGTH 256
#define PLAY 112
#define CREATE 110
#define LIST 108
#define JOIN 106

ClientManager::ClientManager(Socket socket, ProtectedMap& m) : 
    communicator(std::move(socket)), map(m){
        this->finish = false;
        this->simbol = ' ';
        this->queue = nullptr;
    }

void ClientManager::run(){
    while (!this->finish.load()){
        switch (this->communicator.receive_first()){
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
    std::string game;
    this->communicator.receive(game);
    this->queue = this->map.insert(game);
    this->simbol = 'O';
}

void ClientManager::list(){
    std::string allGames("  Partidas:");
    std::string separator("\n - ");
    this->map.get_keys(allGames, separator);
    allGames.push_back('\n');
    this->communicator.send(allGames);
}

void ClientManager::join_game(){
    std::string game;
    this->communicator.receive(game);
    this->map.find(game, this->queue);
    this->simbol = 'X';
    
    std::vector<int> values = this->queue->pop();
    this->board.put(values[0], values[1], values[2]);
}

void ClientManager::play(){
    std::vector<int> values = this->communicator.receive_play();
    values.push_back(this->simbol);
    this->queue->push(values);
    if (this->board.put(values[0], values[1], this->simbol) == 1) return;
    std::vector<int> otherPlayerValues = this->queue->pop();
    this->board.put(otherPlayerValues[0], 
            otherPlayerValues[1], otherPlayerValues[2]);
}

void ClientManager::print_board(){
    std::string string_board("  ");
    if (this->board.print(string_board, this->simbol) == 1){
        this->finish = true;
    }
    this->communicator.send(string_board);
}

bool ClientManager::gameFinish(){
    return this->finish.load();
}
