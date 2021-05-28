#include "socket_exception.h"

const char* SocketException::what() const throw() {
    return this->error.c_str();
}