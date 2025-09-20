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

// Максимальный размер буфера
#define BUFFER_SIZE 65536

int main() {
    RawSocket rawSocket;

    rawSocket.Socket();
    std::cout << "Сниффер запущен. Ожидание пакетов..." << std::endl;

    

    /*int raw_socket;
    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);
    unsigned char (*buffer) = new unsigned char[BUFFER_SIZE];

    // Создаём raw-сокет
    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_socket < 0) {
        perror("Socket error");
        return 1;
    }

    std::cout << "Сниффер запущен. Ожидание пакетов..." << std::endl;

    while (true) {
        ssize_t data_size = recvfrom(raw_socket, buffer, BUFFER_SIZE, 0, &saddr, &saddr_len);
        if (data_size < 0) {
            perror("Recvfrom error");
            break;
        }

        // Указатель на IP-заголовок (после Ethernet-заголовка 14 байт)
        struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));

        struct in_addr src_addr, dest_addr;
        src_addr.s_addr = ip->saddr;
        dest_addr.s_addr = ip->daddr;

        std::cout << "IP-пакет: "
                  << inet_ntoa(src_addr)
                  << " -> "
                  << inet_ntoa(dest_addr)
                  << " | Протокол: "
                  << (int)ip->protocol
                  << std::endl;
    }

    close(raw_socket);
    delete[] buffer;
    return 0;*/
    return 0;
}