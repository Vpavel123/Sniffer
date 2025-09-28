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
#include "ConsoleState.h"

// Максимальный размер буфера
#define BUFFER_SIZE 65536

int main() {
    Logger& log = Logger::getInstance();
    RawSocket rawSocket(log);
    PacketHandler packetHandler;
    rawSocket.init();
    log.Info("Сниффер запущен. Ожидание пакетов");
    int choise;

    Menu menu;
    menu.handleChoice(rawSocket, packetHandler);

    return 0;
}