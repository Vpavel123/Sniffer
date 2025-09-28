#include "PacketHandler.h"

PacketHandler::Packet PacketHandler::parseBuffer(const unsigned char *packet_data, ssize_t data_size)
{
    std::memset(&packet, 0, sizeof(packet));

    if (data_size  < sizeof(struct ethhdr))
    {
        std::cout << "Packet too small for Ethernet header" << std::endl;
        return packet;
    }
    
    // Парсинг Ethernet заголовка
    packet.eth = (struct ethhdr*)packet_data;
    packet.ethertype = ntohs(packet.eth->h_proto);
    
    const unsigned char* ip_data = packet_data + sizeof(struct ethhdr);
    size_t ip_data_size = data_size - sizeof(struct ethhdr);
    
    // Определение версии IP
    if (packet.ethertype == ETH_P_IP && ip_data_size >= sizeof(struct iphdr)) {
        // IPv4
        packet.ipv4 = (struct iphdr*)ip_data;
        packet.ip_version = 4;
        packet.src_ipv4 = { packet.ipv4->saddr };
        packet.dest_ipv4 = { packet.ipv4->daddr };
        packet.transport_protocol = packet.ipv4->protocol;
        
        // Парсинг транспортного уровня
        size_t ip_header_len = packet.ipv4->ihl * 4;
        const unsigned char* transport_data = ip_data + ip_header_len;
        size_t transport_data_size = ip_data_size - ip_header_len;
        
        if (packet.transport_protocol == IPPROTO_TCP && transport_data_size >= sizeof(struct tcphdr)) {
            packet.tcp = (struct tcphdr*)transport_data;
        } else if (packet.transport_protocol == IPPROTO_UDP && transport_data_size >= sizeof(struct udphdr)) {
            packet.udp = (struct udphdr*)transport_data;
        }
        
    } else if (packet.ethertype == ETH_P_IPV6 && ip_data_size >= sizeof(struct ip6_hdr)) {
        // IPv6
        packet.ipv6 = (struct ip6_hdr*)ip_data;
        packet.ip_version = 6;
        packet.src_ipv6 = packet.ipv6->ip6_src;
        packet.dest_ipv6 = packet.ipv6->ip6_dst;
        packet.transport_protocol = packet.ipv6->ip6_ctlun.ip6_un1.ip6_un1_nxt;
        
        // Парсинг транспортного уровня для IPv6 (упрощенный)
        size_t ip_header_len = sizeof(struct ip6_hdr);
        const unsigned char* transport_data = ip_data + ip_header_len;
        size_t transport_data_size = ip_data_size - ip_header_len;
        
        if (packet.transport_protocol == IPPROTO_TCP && transport_data_size >= sizeof(struct tcphdr)) {
            packet.tcp = (struct tcphdr*)transport_data;
        } else if (packet.transport_protocol == IPPROTO_UDP && transport_data_size >= sizeof(struct udphdr)) {
            packet.udp = (struct udphdr*)transport_data;
        }
    }
    
    return packet;
}

PacketHandler::PacketHandler()
{
    buffer = new unsigned char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::memset(&packet, 0, sizeof(packet));
    _ipversion = IPVersion::ANY;
}

PacketHandler::~PacketHandler() {
    delete[] buffer;
}

unsigned char* PacketHandler::getBuffer() const {
    return buffer;
}

uint16_t PacketHandler::getCurrentIPVersion() const {
    return packet.ethertype;
}

bool PacketHandler::isIpv4() const{
    return packet.ip_version == 4;
};

bool PacketHandler::isIpv6() const{
    return packet.ip_version == 6;
};