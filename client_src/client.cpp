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

void Client::start(const char* host, const char* port){
    this->socket.connect(host, port);

    while (std::cin.good()) {
        std::string input_line;
        char buffer[MAX_BUFF_LENGTH], server_answer[MAX_BUFF_LENGTH];
        short firstByte;

        getline(std::cin, input_line);
        if ((firstByte = parse(input_line)) < 0) {
            std::cerr << "Error al ingresar comando." << std::endl;
            continue;
        }

        int buffer_length = fillBuffer(firstByte, input_line, buffer);
        if (this->socket.send(buffer, buffer_length) < 0) break;

        int msg_length = receive_length();
        this->socket.receive(server_answer, msg_length);
        std::cout << server_answer;
    }
}

int Client::receive_length(){
    char buffer[2];
    this->socket.receive(buffer, 2);
    union{
        short number;
        char buffer[2];
    }byte;
    byte.number = 0;
  
    byte.buffer[0] = buffer[0];
    byte.buffer[1] = buffer[1];

    return ntohs(byte.number);
}

//PRE: el cliente manda crear o unirse o jugar seguido de un espacio
static int parse(std::string& line){
    std::vector<std::string> vector = {"jugar ", "crear ", "listar", "unirse "};
    for (unsigned int i = 0; i < vector.size(); i++){
        if (line.compare(0, vector[i].size(), vector[i]) == 0){
            line.erase(0, vector[i].size());
            return 112 - i * 2;
        }
    }
    return -1;
}

int Client::fillBuffer(const short firstByte, std::string& line, char* buffer){
    union{
        short number;
        char buffer[2];
    }byte;

    byte.number = htons(firstByte);
    buffer[0] = byte.buffer[1];

    if (firstByte == 112){
        buffer[1] = line[0] - 1;
        buffer[2] = line[2] - 1;
        return 3; // deberian ser dos
    }
    if(firstByte != 108){
        byte.number = htons(line.size());

        buffer[1] = byte.buffer[0];
        buffer[2] = byte.buffer[1];

        for (unsigned int i = 0; i < line.size(); i++){
            buffer[i + 3] = line[i];
        }
        return line.size() + 3; //los primeros 3 bytes
    }
    return 1;
}

Client::~Client(){
    this->socket.close();
}
