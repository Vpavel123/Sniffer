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
#include "Packet.h"

// Максимальный размер буфера
#define BUFFER_SIZE 65536

int main() {
    Logger& log = Logger::getInstance();
    RawSocket rawSocket(log);
    PacketHandler packet(log);
    rawSocket.Socket();
    std::cout << "Сниффер запущен. Ожидание пакетов..." << std::endl;
    rawSocket.ListInterfaces();
    rawSocket.BindInterface("enp0s3");
    while (true) {
        ssize_t data_size = rawSocket.Recieve(packet.getBuffer(), BUFFER_SIZE,0); 
        if (data_size < 0) {
            log.Error("Recvfrom error");
            break;
        }
        
        packet.FilterProtocol(TCP);
    }
    return 0;
}