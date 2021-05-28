#ifndef __SOCKET_EXCEPTION_H__
#define __SOCKET_EXCEPTION_H__

#include <exception>
#include <string>

class SocketException : public std::exception {
    private:
        std::string error;
    public:
        explicit SocketException(const std::string& e) : error(e) {}
        virtual const char* what() const throw();
};
#endif
