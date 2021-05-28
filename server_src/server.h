#ifndef __SERVER_H__
#define __SERVER_H__

#include "../common_src/socket.h"

class Server{
    private:
        Socket server;
        //esto tiene que ser distinto obviamente
        //solo lo hago para 1 cliente para resolver
        //funciones mas facilmente
        Socket client;

    public:
        Server(const char* port);
        void start(const char* port);
        ~Server();
};
#endif
