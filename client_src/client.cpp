#include "client.h"
#include "../common_src/socket.h"
#include "../common_src/socket_exception.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <arpa/inet.h>

#define MAX_BUFF_LENGTH 256

static int parse(std::string& line);
static void recieve_lenght(Socket* socket, int& msg_lenght);

void Client::start(const char* host, const char* port){
    Socket socket;
    socket.connect(host, port);


    while (std::cin.good()) {
        std::string input_line;
        char buffer[MAX_BUFF_LENGTH];
        short firstByte;
        getline(std::cin, input_line);
        if ((firstByte = parse(input_line)) < 0) {
            std::cerr << "Error al ingresar comando." << std::endl;
            continue;
        }
        fillBuffer(firstByte, input_line, buffer);
        socket.send(buffer, sizeof(buffer));
        int msg_lenght
        recieve_lenght(&socket, msg_lenght);
        socket.recieve(buffer, msg_lenght);
  }
}

static void recieve_lenght(Socket* socket, int& msg_lenght){
    char buffer[2];
    socket->recieve(buffer, 2);
    union{
        short number;
        char buffer[2];
    }byte;
    byte.number = 0;
  
    byte.received_number[0] = buffer[0];
    byte.received_number[1] = buffer[1];
    msg_lenght = (int) ntohs(byte.number);
}

//PRE: el cliente manda crear o unirse o jugar seguido de un espacio
static int parse(std::string& line){
    std::vector<std::string> vector = {"jugar ", "crear ", "listar", "unirse "};
    for (unsigned int i = 0; i < vector.size(); i++){
        if (line.compare(0, vector[i].size(), vector[i])){
            line.erase(0, vector[i].size());
            return 112 - i * 2;
        }
    }
    return -1;
}

void Client::fillBuffer(const short firstByte, std::string& line, char* buffer){
    union{
        short number;
        char buffer[2];
    }byte;

    byte.number = htons(firstByte);
    buffer[0] = byte.buffer[1];

    if (firstByte == 112){
        //fillPlayBuffer(line);
        return;
    }
    if(firstByte != 108){
        byte.number = htons(short (line.size()));

        buffer[1] = byte.buffer[0];
        buffer[2] = byte.buffer[1];

        for (unsigned int i = 0; i < line.size(); i++){
            byte.number = htons(short (line[i]));
            buffer[i + 3] = byte.buffer[1];
        }
    }
}
