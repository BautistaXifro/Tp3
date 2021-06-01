#include "client.h"
#include "../common_src/socket.h"
#include "../common_src/socket_exception.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdint.h>

#define MAX_BUFF_LENGTH 256
#define PLAY 112
#define CREATE 110
#define LIST 108
#define JOIN 106

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

        this->socket.send(buffer, buffer_length);

        int msg_length = receive_length();
        int bytes_receive = this->socket.receive(server_answer, msg_length);
        std::cout << server_answer;

        if (firstByte == CREATE || firstByte == JOIN){
            playState(bytes_receive);
            break;
        }
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
    std::vector<std::string> vector = {"crear ", "listar", "unirse "};
    for (unsigned int i = 0; i < vector.size(); i++){
        if (line.compare(0, vector[i].size(), vector[i]) == 0){
            line.erase(0, vector[i].size());
            return 110 - i * 2;
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
        uint8_t firstValue = line[0] - 49;
        uint8_t secondValue = line[2] - 49;
        uint8_t finalValue = firstValue << 4 | secondValue;
        buffer[1] = finalValue;
        return 2;
    }
    if (firstByte != 108){
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

void Client::playState(const int& bytesCheck){
    bool gameFinish = false;
    while (!gameFinish){
        char buffer[MAX_BUFF_LENGTH], server_answer[MAX_BUFF_LENGTH];
        std::string input_line;
        short firstByte;
        getline(std::cin, input_line);
        if (input_line.compare(0, 6, "jugar ") != 0){
            std::cerr << "Error al ingresar comando."
                     << std::endl;
            continue;
        }
        input_line.erase(0, 6);
        firstByte = PLAY;
        int buffer_length = fillBuffer(firstByte, input_line, buffer);

        socket.send(buffer, buffer_length);
        int msg_length = receive_length();
        int bytes_receive = this->socket.receive(server_answer, msg_length);
        std::cout << server_answer;

        if (bytes_receive > bytesCheck){
            gameFinish = true;
        }
    }
}

Client::~Client(){
    this->socket.close();
}
