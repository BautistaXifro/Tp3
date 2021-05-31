#ifndef __SERVER_H__
#define __SERVER_H__

#include "../common_src/socket.h"
#include "thread.h"
#include "protected_map.h"
#include "client_manager.h"
#include <vector>
#include <atomic>

class Server : public Thread {
    private:
        Socket server_socket;
        std::vector<ClientManager*> clients;
        std::atomic<bool> end;
        ProtectedMap map;
    public:
        Server(const char* port);
        void run() override;
        void remove_client();
        void stop();
};
#endif
