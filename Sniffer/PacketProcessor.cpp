#include "PacketProcessor.h"

void TCPProcessor::transmit(PacketHandler::IPVersion vers){
if (_packetHandler.packet.ip_version == 4 && (vers == PacketHandler::IPv4 || vers == PacketHandler::ANY) &&
               _packetHandler.packet.tcp != nullptr) {
        std::cout << "\n=== TCP Packet (IPv4) ===" << std::endl;
        printf("Source Port: %d\n", ntohs(_packetHandler.packet.tcp->source));
        printf("Destination Port: %d\n", ntohs(_packetHandler.packet.tcp->dest));
        printf("Source IP: %s\n", inet_ntoa(_packetHandler.packet.src_ipv4));
        printf("Destination IP: %s\n", inet_ntoa(_packetHandler.packet.dest_ipv4));
    } else if (_packetHandler.packet.ip_version == 6 && (vers == PacketHandler::IPv6 || vers == PacketHandler::ANY)&&
               _packetHandler.packet.tcp != nullptr) {
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

void UDPProcessor::transmit(PacketHandler::IPVersion vers){
    // Проверяем, что это IPv4 пакет и у нас есть UDP заголовок
    if (_packetHandler.packet.ip_version == 4 && 
        (vers == PacketHandler::IPv4 || vers == PacketHandler::ANY) &&
        _packetHandler.packet.udp != nullptr) {
        
        std::cout << "\n=== UDP Packet (IPv4) ===" << std::endl;
        printf("Source Port: %d\n", ntohs(_packetHandler.packet.udp->source));
        printf("Destination Port: %d\n", ntohs(_packetHandler.packet.udp->dest));
        
        // Проверяем, что IP адреса инициализированы
        if (_packetHandler.packet.src_ipv4.s_addr != 0) {
            printf("Source IP: %s\n", inet_ntoa(_packetHandler.packet.src_ipv4));
        } else {
            printf("Source IP: N/A\n");
        }
        
        if (_packetHandler.packet.dest_ipv4.s_addr != 0) {
            printf("Destination IP: %s\n", inet_ntoa(_packetHandler.packet.dest_ipv4));
        } else {
            printf("Destination IP: N/A\n");
        }
        
    } else if (_packetHandler.packet.ip_version == 6 && 
               (vers == PacketHandler::IPv6 || vers == PacketHandler::ANY) &&
               _packetHandler.packet.udp != nullptr) {
        
        std::cout << "\n=== UDP Packet (IPv6) ===" << std::endl;
        printf("Source Port: %d\n", ntohs(_packetHandler.packet.udp->source));
        printf("Destination Port: %d\n", ntohs(_packetHandler.packet.udp->dest));
        
        char src_ip[INET6_ADDRSTRLEN], dst_ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &_packetHandler.packet.src_ipv6, src_ip, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &_packetHandler.packet.dest_ipv6, dst_ip, INET6_ADDRSTRLEN);
        printf("Source IP: %s\n", src_ip);
        printf("Destination IP: %s\n", dst_ip);
        
    } else {
        std::cout << "Invalid UDP packet or missing headers" << std::endl;
    }
}
void AllProtocolsProcessor::transmit(PacketHandler::IPVersion vers){
    // Определяем тип пакета и вызываем соответствующий процессор
    if (_packetHandler.packet.transport_protocol == IPPROTO_TCP && _packetHandler.packet.tcp != nullptr) {
        // Создаем временный TCP процессор
        TCPProcessor tempProcessor(_packetHandler);
        tempProcessor.transmit(vers);
    } 
    else if (_packetHandler.packet.transport_protocol == IPPROTO_UDP && _packetHandler.packet.udp != nullptr) {
        // Создаем временный UDP процессор
        UDPProcessor tempProcessor(_packetHandler);
        tempProcessor.transmit(vers);
    }
    else {
        // Выводим базовую информацию о пакете
        std::cout << "\n=== Unknown/Other Protocol Packet ===" << std::endl;
        std::cout << "IP Version: " << (int)_packetHandler.packet.ip_version << std::endl;
        std::cout << "Protocol: " << (int)_packetHandler.packet.transport_protocol << std::endl;
        
        if (_packetHandler.packet.ip_version == 4) {
            if (_packetHandler.packet.src_ipv4.s_addr != 0) {
                printf("Source IP: %s\n", inet_ntoa(_packetHandler.packet.src_ipv4));
            }
            if (_packetHandler.packet.dest_ipv4.s_addr != 0) {
                printf("Destination IP: %s\n", inet_ntoa(_packetHandler.packet.dest_ipv4));
            }
        }
        else if (_packetHandler.packet.ip_version == 6) {
            char src_ip[INET6_ADDRSTRLEN], dst_ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &_packetHandler.packet.src_ipv6, src_ip, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET6, &_packetHandler.packet.dest_ipv6, dst_ip, INET6_ADDRSTRLEN);
            printf("Source IP: %s\n", src_ip);
            printf("Destination IP: %s\n", dst_ip);
        }
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