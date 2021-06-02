#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__
#include "thread.h"
#include "board.h"
#include "../common_src/socket.h"
#include "protected_map.h"
#include <atomic>

class ClientManager : public Thread{
    private:
        Socket socket_client;
        ProtectedMap& map;
        ProtectedQueue* queue;
        Board board;
        char simbol;
        std::atomic<bool> finish;
    public:
        ClientManager(Socket socket, ProtectedMap& map);
        void print_board();
        void run() override;
        void create();
        void list();
        void join_game();
        void play();
        bool gameFinish();
        ~ClientManager();
};
#endif
