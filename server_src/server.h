#ifndef __SERVER_H__
#define __SERVER_H__

#include "../common_src/socket.h"
#include "thread.h"
#include "protected_map.h"
#include <vector>

class Server{
    private:
        Socket socket_server;
        //esto tiene que ser distinto obviamente
        //solo lo hago para 1 cliente para resolver
        //funciones mas facilmente
        Socket socket_client;
        std::vector<Thread*> clients;
        ProtectedMap map;

    public:
        Server(const char* port);
        void start(const char* port);
        void find_and_print_board(const std::string& game);
        void run();
        void create();
        void list();
        void join_game();
        void play();
        ~Server();
};
#endif
