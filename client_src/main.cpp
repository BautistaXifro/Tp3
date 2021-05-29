#include "client.h"
#include <iostream>

#define UNKNOWN_ERROR "Se produjo un error desconocido en el cliente"


int main(int argc, char* argv[]){
    try {
        if(argc != 3){
            std::cerr << "Error de argumento." << std::endl;
            return 1;
        }

        Client client;
        client.start(argv[1], argv[2]);
        
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;

    }catch (...) {
        std::cerr << UNKNOWN_ERROR << std::endl;
        return 1;
    }
    return 0;
}