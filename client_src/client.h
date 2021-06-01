#ifndef __CLIENT_H__
#define __CLIENT_H__
#include <string>
#include "../common_src/socket.h"

class Client {
    private:
        Socket socket;
    public:
        void start(const char* host, const char* port);
        int fillBuffer(const short value, std::string& line, char* buffer);
        int receive_length();
        void playState(int& bytesCheck);
        ~Client();
};
#endif
