#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "RawSocket.h"
#include "PacketHandler.h"
#include "PacketProcessor.h"

// Максимальный размер буфера
#define BUFFER_SIZE 65536

int main() {
    Logger& log = Logger::getInstance();
    RawSocket rawSocket(log);
    PacketHandler packetHandler;
    //PacketHandler packet(log);
    rawSocket.init();
    log.Info("Сниффер запущен. Ожидание пакетов");
    rawSocket.listInterfaces();
    rawSocket.bindInterface("enp0s3");
    std::unique_ptr<PacketProcessor> processor = std::make_unique<TCPProcessor>(packetHandler);

    while (true) {
        ssize_t data_size = rawSocket.recieve(packetHandler.getBuffer(), BUFFER_SIZE,0); 
        if (data_size < 0) {
            log.Error("Recvfrom error");
            break;
        }
        packetHandler.parseBuffer(packetHandler.getBuffer(), data_size);
        processor.get()->transmit(packetHandler.IPv4);
        
        /*if (packetHandler.packet.ethertype == ETH_P_IP)
        {
            std::cout << "\n=== TCP Header ===" << std::endl;
            printf("Source Port: %d\n", ntohs(packetHandler.packet.tcp->source));
            printf("Destination Port: %d\n", ntohs(packetHandler.packet.tcp->dest));
            printf("Source IP: %s\n", inet_ntoa(packetHandler.packet.dest_ipv4));
            printf("Destination IP: %s\n", inet_ntoa(packetHandler.packet.src_ipv4));
        }*/
        
    }
    return 0;
}