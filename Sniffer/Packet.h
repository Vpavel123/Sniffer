#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "Log.h"

#define BUFFER_SIZE 65536

enum Protocol{
    TCP = 6,
    UDP = 17,
    ICMP = 1,
    ALL = 0
};

class PacketHandler
{
private:
    unsigned char *buffer;
public:
    struct Packet{
        struct iphdr *ip;
        struct ethhdr* eth;
        struct tcphdr* tcp;
        struct udphdr* udp;
        struct in_addr src_addr, dest_addr;
    } packet;

    Logger& _log;
    Protocol protocol;
    PacketHandler(Logger& log);
    ~PacketHandler();
    void print_hex(const u_char* data, int len);
    unsigned char* getBuffer() const;
    void FilterProtocol(Protocol protocol);
    void FilterIP();
    void InternetHeader();
    void IpHeader();
};

#endif