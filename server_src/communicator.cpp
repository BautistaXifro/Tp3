#include "communicator.h"
#include <utility>
#include <string>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#define MAX_BUFFER_SIZE 256

Communicator::Communicator(Socket s) : socket_client(std::move(s)){}

int Communicator::send(const std::string& bufToSend){
    char buffer[MAX_BUFFER_SIZE];
    short length = bufToSend.copy(buffer, bufToSend.size(), 0);

    set_to_send_length(buffer, length - 2);// menos 2 byte de longitud

    return this->socket_client.send(buffer, length);
}

int Communicator::receive(std::string& buffer){
    char recvBuffer[MAX_BUFFER_SIZE];

    int bytesRecv = this->socket_client.receive(recvBuffer, get_length());
    buffer = recvBuffer;

    return bytesRecv;
}

int Communicator::receive_first(){
    union{
        short number;
        char buffer[2];
    }byte;

    byte.number = 0;
    char firstByte[1];
    this->socket_client.receive(firstByte, 1);
    byte.buffer[1] = firstByte[0];
    return ntohs(byte.number);
}

std::vector<int> Communicator::receive_play(){
    char buffer[1];
    this->socket_client.receive(buffer, 1);
    uint8_t value = buffer[0];
    int column = (int) value >> 4;
    int row = (int) value & 15;
    int aux[] = {column, row};
    std::vector<int> vector(aux, aux + 2);
    return vector;
}

void Communicator::set_to_send_length(char* buffer, const short& length){
    union{
        short number;
        char buffer[2];
    }byte;
    byte.number = 0;
    
    byte.number = htons(length);
    buffer[0] = byte.buffer[0];
    buffer[1] = byte.buffer[1];
}

int Communicator::get_length(){
    char length[2];
    this->socket_client.receive(length, 2);

    short number = *(short*)(length);
    return ntohs(number);
}

Communicator::~Communicator(){
    this->socket_client.close();
}
