#ifndef RAWSOCKET_H
#define RAWSOCKET_H

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include "Log.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>

#define MAX_IFS 64

class RawSocket
{
public:
    Logger& _log;
    int raw_socket;
    struct sockaddr saddr;
    socklen_t saddr_len;

    struct Interface
    {
        struct ifconf ifc;
        struct ifreq ifr[MAX_IFS];
        struct sockaddr_ll sll;
    } interface;


    RawSocket(Logger& log);
    ~RawSocket();
    RawSocket(const RawSocket& order) = delete;
    RawSocket& operator = (const RawSocket& order) = delete;

    void Socket();
    void BindInterface(const std::string& name);
    void ListInterfaces();
    int Recieve(unsigned char *buffer, size_t lenght, int flag);
    void Close();
private:
};

#endif