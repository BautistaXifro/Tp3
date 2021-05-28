#ifndef __CLIENT_H__
#define __CLIENT_H__
#include <string>

class Client {
    public:
        void start(const char* host, const char* port);
        void fillBuffer(const short value, std::string& line, char* buffer);
};
#endif
