#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "Log.h"

class RawSocket
{
public:
    Logger& log = Logger::getInstance();
    int raw_socket;
    struct sockaddr saddr;
    socklen_t saddr_len;

    RawSocket();
    ~RawSocket();
    RawSocket(const RawSocket& order) = delete;
    RawSocket& operator = (const RawSocket& order) = delete;

    void Socket();
    void Close();
private:
};