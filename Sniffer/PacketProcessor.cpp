#include "PacketProcessor.h"

void TCPProcessor::transmit(PacketHandler::IPVersion vers){
if (_packetHandler.packet.ip_version == 4 && (vers == PacketHandler::IPv4 || vers == PacketHandler::ANY)) {
        std::cout << "\n=== TCP Packet (IPv4) ===" << std::endl;
        printf("Source Port: %d\n", ntohs(_packetHandler.packet.tcp->source));
        printf("Destination Port: %d\n", ntohs(_packetHandler.packet.tcp->dest));
        printf("Source IP: %s\n", inet_ntoa(_packetHandler.packet.src_ipv4));
        printf("Destination IP: %s\n", inet_ntoa(_packetHandler.packet.dest_ipv4));
    } else if (_packetHandler.packet.ip_version == 6 && (vers == PacketHandler::IPv6 || vers == PacketHandler::ANY)) {
        std::cout << "\n=== TCP Packet (IPv6) ===" << std::endl;
        printf("Source Port: %d\n", ntohs(_packetHandler.packet.tcp->source));
        printf("Destination Port: %d\n", ntohs(_packetHandler.packet.tcp->dest));
        char src_ip[INET6_ADDRSTRLEN], dst_ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &_packetHandler.packet.src_ipv6, src_ip, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &_packetHandler.packet.dest_ipv6, dst_ip, INET6_ADDRSTRLEN);
        printf("Source IP: %s\n", src_ip);
        printf("Destination IP: %s\n", dst_ip);
    }
}

void UPDProcessor::transmit(PacketHandler::IPVersion vers){
    if (_packetHandler.packet.ip_version == vers || vers == PacketHandler::IPv4)
    {
            std::cout << "\n=== UDP Header ===" << std::endl;
        printf("Source Port: %d\n", ntohs(_packetHandler.packet.udp->source));
        printf("Destination Port: %d\n", ntohs(_packetHandler.packet.udp->dest));
    }
}

std::unique_ptr<PacketProcessor> PacketProcessorFactory::createProcessor(PacketHandler::Protocol protocol, PacketHandler& packetHandler) {
    switch (protocol) {
        case PacketHandler::TCP:
            return std::make_unique<TCPProcessor>(packetHandler);
        case PacketHandler::UDP:
            return std::make_unique<UDPProcessor>(packetHandler);
        case PacketHandler::ALL:
            return std::make_unique<AllProtocolsProcessor>(packetHandler);
        default:
            return std::make_unique<AllProtocolsProcessor>(packetHandler);
    }
}


/*void TCPProcessor::printPacket() {
    _packetHandler.packet.ipv4 = (struct iphdr*)(_packetHandler.getBuffer() + sizeof(struct ethhdr));
    _packetHandler.packet.src_addr.s_addr = _packetHandler.packet.ipv4->saddr;
    _packetHandler.packet.dest_addr.s_addr = _packetHandler.packet.ipv4->daddr;

    _packetHandler.packet.tcp = (struct tcphdr*)(_packetHandler.getBuffer() + sizeof(struct ethhdr) + (_packetHandler.packet.ipv4->ihl * 4));
    
    std::cout << "\n=== TCP Header ===" << std::endl;
    printf("Source Port: %d\n", ntohs(_packetHandler.packet.tcp->source));
    printf("Destination Port: %d\n", ntohs(_packetHandler.packet.tcp->dest));
    printf("Source IP: %s\n", inet_ntoa(_packetHandler.packet.src_addr));
    printf("Destination IP: %s\n", inet_ntoa(_packetHandler.packet.dest_addr));
}

void TCPProcessor::processPacket(const unsigned char* packet_data, ssize_t data_size){
    auto a = getPacketHandler().parseBuffer(packet_data, data_size);

    if (a.ipv4 == )
    {
        
    }
    
}

TCPProcessor::TCPProcessor() {
    // Конструктор
}

TCPProcessor::~TCPProcessor() {
    // Деструктор
}

void UDPProcessor::printPacket() {
    /*_packetHandler.packet.ip = (struct iphdr*)(_packetHandler.getBuffer() + sizeof(struct ethhdr));
    _packetHandler.packet.src_addr.s_addr = _packetHandler.packet.ip->saddr;
    _packetHandler.packet.dest_addr.s_addr = _packetHandler.packet.ip->daddr;

    _packetHandler.packet.udp = (struct udphdr*)(_packetHandler.getBuffer() + sizeof(struct ethhdr) + (_packetHandler.packet.ip->ihl * 4));
    
    std::cout << "\n=== UDP Header ===" << std::endl;
    printf("Source Port: %d\n", ntohs(_packetHandler.packet.udp->source));
    printf("Destination Port: %d\n", ntohs(_packetHandler.packet.udp->dest));
    printf("Source IP: %s\n", inet_ntoa(_packetHandler.packet.src_addr));
    printf("Destination IP: %s\n", inet_ntoa(_packetHandler.packet.dest_addr));
}

UDPProcessor::UDPProcessor() {
    // Конструктор
}

UDPProcessor::~UDPProcessor() {
    // Деструктор
}

void InternetProtocolProcessor::printPacket() {

}

void InternetProtocolProcessor::InternetPacket(){
    getPacketHandler().packet.ipv4 = (struct iphdr*)(getPacketHandler().getBuffer() + sizeof(struct ethhdr));
    getPacketHandler().packet.src_addr.s_addr = packet.ip->saddr;
    getPacketHandler().packet.dest_addr.s_addr = packet.ip->daddr;
}

InternetProtocolProcessor::InternetProtocolProcessor() {
    // Конструктор
}

InternetProtocolProcessor::~InternetProtocolProcessor() {
    // Деструктор
}

uint16_t InternetProtocolProcessor::InternetProtocol(PacketHandler::IPVersion vers) const{
    return PacketHandler::Packet::ethertype = (uint16_t)vers;
}*/


