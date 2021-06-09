#ifndef __COMUNICATOR_H__
#define __COMUNICATOR_H__
#include "../common_src/socket.h"
#include <string>
#include <vector>
class Communicator {
    private:
        Socket socket_client;
    public:
        explicit Communicator(Socket socket);
        int send(const std::string& bufToSend);
        int receive(std::string& buffer);
        int receive_first();
        std::vector<int> receive_play();
        void set_to_send_length(char* buffer, const short& length);
        int get_length();
        ~Communicator();
};
#endif
