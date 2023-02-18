#ifndef MY_CHAT_SOCKET

#define MY_CHAT_SOCKET


class InetAddress;

class Socket
{
    public:
        explicit Socket(int sockfd) 
            : sockfd_(sockfd)
        {
            
        }

        ~Socket();

        int fd() const {return sockfd_;}

        void bindAddress(const InetAddress& localadrr);

        void listen();

        int accept(InetAddress* peeraddr);

        void setReuseAddr(bool on);

    private:
        const int sockfd_;
};


#endif //MY_CHAT_SOCKET