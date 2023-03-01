#ifndef MY_CHAT_INETADDRESS

#define MY_CHAT_INETADDRESS

#include <string>
#include <netinet/in.h>

class InetAddress
{
    public:
        explicit InetAddress(uint16_t port);

        InetAddress(const std::string& ip, uint16_t port);

        InetAddress(const struct sockaddr_in& addr)
            : addr_(addr)
        {
            
        }

        std::string toHostPort() const;

        const struct sockaddr_in& getSockAddrInet() const { return addr_; }
        void setSockAddrInet(const sockaddr_in& addr) { addr_ = addr; }

    private:
        struct sockaddr_in addr_;
};


#endif //MY_CHAT_INETADDRESS