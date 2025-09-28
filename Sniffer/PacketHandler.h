#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define BUFFER_SIZE 65536

class PacketHandler
{
public:
    struct Packet {
        // Ethernet
        struct ethhdr* eth;
        
        // IP заголовки (может быть IPv4 или IPv6)
        struct iphdr* ipv4;
        struct ip6_hdr* ipv6;
        
        // Транспортные протоколы
        struct tcphdr* tcp;
        struct udphdr* udp;
        
        // Адреса
        struct in_addr src_ipv4, dest_ipv4;
        struct in6_addr src_ipv6, dest_ipv6;
        
        // Метаданные
        uint8_t ip_version;
        uint8_t transport_protocol;
        uint16_t ethertype;
    } packet;


    enum Protocol{
        TCP = 6,
        UDP = 17,
        ICMP = 1,
        ALL = 0
    };

    enum IPVersion{
        ANY = 0,
        IPv4 = 4,
        IPv6 = 6
    };

    unsigned char* getBuffer() const;

    PacketHandler::Packet parseBuffer(const unsigned char* packet_data, ssize_t data_size);
    uint16_t getCurrentIPVersion() const;
    PacketHandler();
    ~PacketHandler();
    
    bool isIpv4() const;
    bool isIpv6() const;
private:
    unsigned char *buffer;
    IPVersion _ipversion;
};

#endif