#ifndef __SERVER_H__
#define __SERVER_H__

#include "../common_src/socket.h"
#include "thread.h"
#include "protected_map.h"
#include "client_manager.h"
#include <vector>

class Server : public Thread {
    private:
        Socket server_socket;
        std::vector<ClientManager*> clients;
        ProtectedMap map;
    public:
        explicit Server(const char* port);
        void run() override;
        void remove_client();
        void stop();
        ~Server();
};
#endif
