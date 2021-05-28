#include "socket.h"
#include "socket_exception.h"

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <exception>

#define GETADDRINFO_ERROR "Error al obtener el getaddrinfo del cliente"
#define BIND_ERROR "Error en socket bind"
#define ACCEPT_ERROR "Error en socket accept"
#define SEND_ERROR "Error en socket send"
#define RECEIVE_ERROR "Error en socket receive"
#define CONNECT_ERROR "Error en socket connect"

static int _getaddrinfo(struct addrinfo** serv_info,
             const char* hostname, const char* port);


static int _getaddrinfo(struct addrinfo** serv_info,
             const char* hostname, const char* port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    int error = getaddrinfo(hostname, port, &hints, serv_info);
    if (error) {
        std::cerr << "Error getaddrinfo." << std::endl;
        return -1;
    }

    return 0;
}

int Socket::connect(const char* hostname, const char* port){
    struct addrinfo* serv_info;

    if (_getaddrinfo(&serv_info, hostname, port)) {
        throw std::invalid_argument(GETADDRINFO_ERROR);
    }

    struct addrinfo* head = serv_info;
    int connected = 0;

    for (; serv_info && !connected; serv_info = serv_info->ai_next) {
        int fd = socket(serv_info->ai_family, serv_info->ai_socktype,
                        serv_info->ai_protocol);
        if (::connect(fd, serv_info->ai_addr,
                    serv_info->ai_addrlen) == -1) {
            ::close(fd);
        } else {
            this->fd = fd;
            connected = 1;
        }
    }

    freeaddrinfo(head);

    if(!connected){
        throw SocketException(CONNECT_ERROR);
    }

    return 0;
}

int Socket::bind(const char* port) {
    struct addrinfo* serv_info;

    if (_getaddrinfo(&serv_info, NULL, port)) {
        throw SocketException(BIND_ERROR);
    }

    int connected = 0;
    struct addrinfo* head = serv_info;

    for (; serv_info && !connected; serv_info = serv_info->ai_next) {
        int fd = socket(serv_info->ai_family, serv_info->ai_socktype,
                         serv_info->ai_protocol);

        int val = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

        if (::bind(fd, serv_info->ai_addr, serv_info->ai_addrlen) < 0){
            ::close(fd);
        } else {
            this->fd = fd;
            connected = 1;
        }
    }

    listen(this->fd, 10);
    freeaddrinfo(head);

    if (!connected) {
        throw SocketException(BIND_ERROR);
    }

    return 0;
}

Socket Socket::accept() {
    struct sockaddr_in address;
    socklen_t address_length = (socklen_t)sizeof(address);

    int fd = ::accept(this->fd, (struct sockaddr*)&address, &address_length);
    if (fd < 0){
        throw SocketException(ACCEPT_ERROR);
    }

    return Socket(fd);
}

int Socket::send(const char* msg, unsigned int msg_length) {
    if (msg_length == 0) {
        return 0;
    }

    int bytes_pending = msg_length;
    unsigned int total_send = 0;

    while (total_send < msg_length) {
        int bytes = ::send(this->fd, &msg[total_send],
                         bytes_pending, MSG_NOSIGNAL);

        if (bytes == -1){
            throw SocketException(SEND_ERROR);
        }else if (bytes == 0){
            break;
        }

        total_send += bytes;
        bytes_pending -= bytes;
    }

    return total_send;
}

int Socket::receive(char* buffer, int msg_length){    
    int bytes_received = 0;
    int max_bytes = msg_length;

    while (bytes_received < msg_length) {
        int bytes = recv(this->fd, &buffer[bytes_received], max_bytes, 0);

        if (bytes == -1) {
            throw SocketException(RECEIVE_ERROR);
        }

        if (bytes == 0) {
            break;
        }

        bytes_received += bytes;
        max_bytes -= bytes;
    }

    buffer[bytes_received] = 0;

    return bytes_received;
}

void Socket::close() {
    shutdown(this->fd, SHUT_RDWR);
    ::close(this->fd);
    this->fd = -1;
}

Socket::~Socket() {
    if (this->fd == -1) {
        return;
    }
    ::close(this->fd);
}
