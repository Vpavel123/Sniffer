#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "Log.h"

#define BUFFER_SIZE 65536

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

    enum Protocol{
        TCP = 6,
        UDP = 17,
        ICMP = 1,
        ALL = 0
    };

    enum class IPVersion{
        ALL,
        IPv4,
        IPv6
    };

    Logger& _log;
    Protocol protocol;
    PacketHandler(Logger& log);
    ~PacketHandler();
    void print_hex(const u_char* data, int len);
    unsigned char* getBuffer() const;
    void WWW(const ssize_t data_size);
    void FilterProtocol(Protocol protocol, const ssize_t data_size);
    void SelectProtocol(Protocol protocol);
    void PortProtocol(Protocol protocol);
    void FilterIP();
    void InternetHeader();
    void IpHeader();

    bool isIpv4() const;
    bool isIpv6() const;
};

#endif