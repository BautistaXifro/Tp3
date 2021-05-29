#ifndef __SOCKET_H__
#define __SOCKET_H__

class Socket{
    private:
        int fd;
        explicit Socket(int specificFd) : fd(specificFd) {}
    public:
        Socket(Socket&& other);
        Socket& operator=(Socket&& other);
        Socket() : fd(-1) {};
        int connect(const char* hostname, const char* port);
        int bind(const char* port);
        Socket accept();
        int send(const char* msg,unsigned int msg_length);
        int receive(char* buffer, int msg_length);
        void close();
        ~Socket();
};
#endif
