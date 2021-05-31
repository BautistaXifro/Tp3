#include "server.h"
#include "../common_src/socket_exception.h"
#include <vector>


Server::Server(const char* port){
    this->server_socket.bind(port);
    this->end = false;
}

void Server::run(){
    try{
        while (!end){
        this->clients.push_back(new ClientManager(
                    std::move(this->server_socket.accept()), this->map));
        this->clients.back()->start();
        remove_client();
        }
    }catch (SocketException& e){
        while (!this->clients.empty()){
            remove_client();
        }
    }
}

void Server::remove_client(){
     std::vector<ClientManager*>::iterator it = this->clients.begin();

    while (it != this->clients.end()) {
      if ((*it)->gameFinish()) {
        (*it)->join();
        delete *it;
        it = this->clients.erase(it);
      } else {
        ++it;
      }
    }
}
void Server::stop(){
    this->end = true;
    this->server_socket.close();
}
