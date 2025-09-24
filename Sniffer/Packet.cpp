#include "Packet.h"

PacketHandler::PacketHandler(Logger& log) : _log(log){ 
    buffer = new unsigned char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
}

unsigned char* PacketHandler::getBuffer() const{
    return buffer;
}

PacketHandler::~PacketHandler(){
    delete[] buffer;
}

void PacketHandler::print_hex(const u_char* data, int len) {
    for (int i = 0; i < len; ++i) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

void PacketHandler::FilterIP(){
    
}

void PacketHandler::IpHeader(){
        _log.Info("\n=== IP Header ===" );
        _log.Info("Source IP: {}", inet_ntoa(*(struct in_addr*)&packet.ip->saddr));
        _log.Info("Dest IP: {}", inet_ntoa(*(struct in_addr*)&packet.ip->daddr));
        _log.Info("Protocol: {}", packet.ip->protocol);

        _log.Info("Version: {}", packet.ip->version);
        _log.Info("Header Length: {} bytes", packet.ip->ihl * 4);
        _log.Info("Total Length: {} bytes", ntohs(packet.ip->tot_len));
        _log.Info("TTL: {}", packet.ip->ttl);
        _log.Info("ID: {}", ntohs(packet.ip->id));
}

void PacketHandler::InternetHeader(){
    packet.eth = (struct ethhdr*)buffer;
    
    _log.Info("=== Ethernet Header ===");
    _log.Info("Source MAC: {}:{}:{}:{}:{}:{}\n",
           packet.eth->h_source[0], packet.eth->h_source[1], packet.eth->h_source[2],
           packet.eth->h_source[3], packet.eth->h_source[4], packet.eth->h_source[5]);
    _log.Info("Dest MAC: {}:{}:{}:{}:{}:{}\n",
           packet.eth->h_dest[0], packet.eth->h_dest[1], packet.eth->h_dest[2],
           packet.eth->h_dest[3], packet.eth->h_dest[4], packet.eth->h_dest[5]);
    _log.Info("Protocol: {}\n", ntohs(packet.eth->h_proto));

    // Проверяем, что это IP-пакет
    if (ntohs(packet.eth->h_proto) != 0x0800) {
        _log.Info("Not an IP packet");
        return;
    }
}

void PacketHandler::FilterProtocol(Protocol protocol){
        packet.ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
        packet.src_addr.s_addr = packet.ip->saddr;
        packet.dest_addr.s_addr = packet.ip->daddr;

        switch (protocol)
        {
        case Protocol::TCP:
            {
                if (packet.ip->protocol == IPPROTO_TCP)
                {
                    IpHeader();
                    /*InternetHeader();
                std::cout << "IP-пакет: "
                  << inet_ntoa(packet.src_addr)
                  << " -> "
                  << inet_ntoa(packet.dest_addr)
                  << " | Протокол: "
                  << (int)packet.ip->protocol
                  << std::endl;*/
                }
                 /*int tcp_header_len = packet.ip->doff * 4;
                    int payload_offset = sizeof(struct ethhdr) + ip_header_len + tcp_header_len;
                    int payload_len = data_size - payload_offset;

                    std::cout << "\n=== Payload (Data) ===\n";
                    print_hex(buffer + payload_offset, payload_len);*/
                
            }
        break;
            case Protocol::UDP:
                {
                if (packet.ip->protocol == IPPROTO_UDP)
                {
                    
                std::cout << "IP-пакет: "
                  << inet_ntoa(packet.src_addr)
                  << " -> "
                  << inet_ntoa(packet.dest_addr)
                  << " | Протокол: "
                  << (int)packet.ip->protocol
                  << std::endl;
                }
            }
            break;
        case Protocol::ICMP:
            {
                if (packet.ip->protocol == IPPROTO_ICMP)
                {
                    
                std::cout << "IP-пакет: "
                  << inet_ntoa(packet.src_addr)
                  << " -> "
                  << inet_ntoa(packet.dest_addr)
                  << " | Протокол: "
                  << (int)packet.ip->protocol
                  << std::endl;
                }
            }
            break;
            case Protocol::ALL:
            {
                std::cout << "IP-пакет: "
                  << inet_ntoa(packet.src_addr)
                  << " -> "
                  << inet_ntoa(packet.dest_addr)
                  << " | Протокол: "
                  << (int)packet.ip->protocol
                  << std::endl;
            }
            break;
        default:
            break;
        }

}