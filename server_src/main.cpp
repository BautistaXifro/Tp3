#include "server.h"
#include "../common_src/socket_exception.h"
#include <iostream>

#define UNKNOWN_ERROR "Se produjo un error desconocido en el servidor"

int main(int argc, char* argv[]){
    try {
        if (argc != 2){
            std::cerr << "Error de argumento." << std::endl;
            return 1;
        }

        Server server(argv[1]);
        server.start();

        char flag;
        while ((flag = getchar()) != 'q'){}
        
        server.stop();
        server.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << UNKNOWN_ERROR << std::endl;
        return 1;
    }
    return 0;
}
