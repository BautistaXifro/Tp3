#include "server.h"
#include <list>

Server::Server(const char* port){
    this->server.bind(port);
    this->clients = this->server.accept());
}


//capaz que tiene que ir en una funcion y no en el destructor
Server::~Server(){
    for(auto client : clients){
        client.close();
    }
    this->server.close();
}
