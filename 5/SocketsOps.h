#ifndef MY_CHAT_SOCKETSOPS

#define MY_CHAT_SOCKETSOPS

#include <arpa/inet.h>
#include <endian.h>

namespace sockets
{
    inline uint64_t hostToNetwork64(uint64_t host64)
    {
        return htobe64(host64);
    }

    inline uint32_t hostToNetwork32(uint32_t host32)
    {
        return htobe32(host32);
    }

    inline uint16_t hostToNetwork16(uint16_t host16)
    {
        return htobe16(host16);
    }

    inline uint64_t networkToHost64(uint64_t net64)
    {
        return be64toh(net64);
    }

    inline uint32_t networkToHost32(uint32_t net32)
    {
        return be32toh(net32);
    }

    inline uint16_t networkToHost16(uint16_t net16)
    {
        return be16toh(net16);
    }

    int createNonblockingOrDie();

    void bindOrDie(int sockfd, const struct sockaddr_in& addr);
    void listenOrDie(int sockfd);
    int accept(int sockfd, struct sockaddr_in* addr);
    void close(int sockfd);

    void toHostPort(char* buf, size_t size, const struct sockaddr_in& addr);
    void fromHostPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

    struct sockaddr_in getLocalAddr(int sockfd);
}


#endif //MY_CHAT_SOCKETSOPS