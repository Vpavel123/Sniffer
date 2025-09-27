#include "Packet.h"

PacketHandler::PacketHandler(Logger& log) : _log(log){ 
    buffer = new unsigned char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
}

bool PacketHandler::isIpv4() const{
    return true;
};

bool PacketHandler::isIpv6() const{
    return true;
};

std::string formatMac(const uint8_t* mac) {
    char buffer[18];
    snprintf(buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(buffer);
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
    
    _log.Info("\n=== Ethernet Header ===");
    _log.Info("Source MAC: {}", formatMac(packet.eth->h_source));
    _log.Info("Dest MAC: {}", formatMac(packet.eth->h_dest));
    _log.Info("Protocol: {}", ntohs(packet.eth->h_proto));

    // Проверяем, что это IP-пакет
    if (ntohs(packet.eth->h_proto) != 0x0800) {
        _log.Info("Not an IP packet");
        return;
    }
}

const char* get_service_name(int port) {
    switch (port) {
        case 20: return "FTP-DATA";
        case 21: return "FTP";
        case 22: return "SSH";
        case 23: return "TELNET";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 110: return "POP3";
        case 143: return "IMAP";
        case 443: return "HTTPS";
        case 993: return "IMAPS";
        case 995: return "POP3S";
        default: return "Unknown";
    }
}
    
void PacketHandler::PortProtocol(Protocol protocol){
    _log.Info("\n=== Service Information ===");
    int dest_port = ntohs(packet.tcp->dest);
    int src_port = ntohs(packet.tcp->source);
    
    _log.Info("Source Port {}: {}", src_port, get_service_name(src_port));
    _log.Info("Dest Port {}: {}", dest_port, get_service_name(dest_port));
}

void PacketHandler::WWW(const ssize_t data_size){
    // 4. Данные (полезная нагрузка)
    int tcp_header_len = packet.tcp->doff * 4;
    int payload_offset = sizeof(struct ethhdr) + (packet.ip->ihl * 4) + tcp_header_len;
    int payload_len = data_size - payload_offset;

    _log.Info("\n=== TCP Data ===");
    _log.Info("Payload offset: {}", payload_offset);
    _log.Info("Payload length: {} bytes", payload_len);

    if (payload_len > 0) {
        std::cout << "First 100 bytes of data (hex):" << std::endl;
        for (int i = 0; i < std::min(payload_len, 100); i++) {
            printf("%02X ", buffer[payload_offset + i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n");

        std::cout << "First 100 bytes of data (ASCII):" << std::endl;
        for (int i = 0; i < std::min(payload_len, 100); i++) {
            unsigned char c = buffer[payload_offset + i];
            if (c >= 32 && c <= 126) {
                printf("%c", c);
            } else {
                printf(".");
            }
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n");
    } else {
        _log.Warning("No payload data");
    }
}

void PacketHandler::SelectProtocol(Protocol protocol){
    if (!packet.ip)
    {
        return;
    }

    if (protocol == TCP)
    {
        packet.tcp = (struct tcphdr*)(buffer + sizeof(struct ethhdr) + (packet.ip->ihl * 4));
        std::cout << "\n=== TCP Header ===" << std::endl;
        printf("Source Port: %d\n", ntohs(packet.tcp->source));
        printf("Dest Port: %d\n", ntohs(packet.tcp->dest));
        printf("Sequence Number: %u\n", ntohl(packet.tcp->seq));
        printf("Acknowledgment Number: %u\n", ntohl(packet.tcp->ack_seq));
        printf("Header Length: %d bytes\n", packet.tcp->doff * 4);
        printf("Window Size: %d\n", ntohs(packet.tcp->window));
        printf("Checksum: 0x%04X\n", ntohs(packet.tcp->check));
        printf("Urgent Pointer: %d\n", ntohs(packet.tcp->urg_ptr));

        // Флаги TCP
        std::cout << "Flags: ";
        if (packet.tcp->urg) std::cout << "URG ";
        if (packet.tcp->ack) std::cout << "ACK ";
        if (packet.tcp->psh) std::cout << "PSH ";
        if (packet.tcp->rst) std::cout << "RST ";
        if (packet.tcp->syn) std::cout << "SYN ";
        if (packet.tcp->fin) std::cout << "FIN ";
        std::cout << std::endl;
    }
    
}

void PacketHandler::FilterProtocol(Protocol protocol, const ssize_t data_size){
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
                    InternetHeader();
                    SelectProtocol(TCP);
                    WWW(data_size);
                    PortProtocol(TCP);
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