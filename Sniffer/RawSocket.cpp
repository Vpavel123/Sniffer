#include "RawSocket.h"

RawSocket::RawSocket(Logger& log) : _log(log){
    saddr_len = sizeof(saddr);
    memset(&interface.sll, 0, sizeof(interface.sll));
}

RawSocket::~RawSocket(){
    Close();
}

void RawSocket::Socket(){
    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_socket < 0) {
        _log.Error("Socket error");
    }
}

int RawSocket::Recieve(unsigned char *buffer, size_t lenght, int flag){
    if (raw_socket < 0) {
        _log.Error("Socket error");
    }
    
    return recvfrom(raw_socket, buffer, lenght, flag, &saddr, &saddr_len);
}

void RawSocket::Close(){
    if (raw_socket != -1) {
        close(raw_socket);
        raw_socket = -1;
        std::cout << "Сокет закрыт" << std::endl;
    }
}

void RawSocket::BindInterface(const std::string& name){
    if (raw_socket < 0) {
        _log.Error("Socket not initialized for binding");
        return;
    }
    
    interface.sll.sll_family = AF_PACKET;
    interface.sll.sll_protocol = htons(ETH_P_ALL);
    interface.sll.sll_ifindex = if_nametoindex(name.c_str());
    
    if (interface.sll.sll_ifindex == 0) {
        _log.Error("Interface " + name + " not found");
        return;
    }

    if (bind(raw_socket, (struct sockaddr*)&interface.sll, sizeof(interface.sll)) < 0) {
        _log.Error("Bind failed for interface " + name);
    } else {
        _log.Info("Успешно привязан к интерфейсу: " + name);
    }
}

void RawSocket::ListInterfaces(){
    // Создаем временный сокет только для получения информации об интерфейсах
    int temp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (temp_socket < 0) {
        _log.Error("Temp socket error");
        return;
    }

    interface.ifc.ifc_len = sizeof(interface.ifr);
    interface.ifc.ifc_buf = (char*)interface.ifr;

    if (ioctl(temp_socket, SIOCGIFCONF, &interface.ifc) < 0) {
        _log.Error("ioctl SIOCGIFCONF");
        Close();
        return;
    }

    int interfaces_count = interface.ifc.ifc_len / sizeof(struct ifreq);
    _log.Info("Доступные интерфейсы (" + std::to_string(interfaces_count) + "):\n");

    for (int i = 0; i < interfaces_count; ++i) {
        std::string name = interface.ifr[i].ifr_name;

        // Получаем IP-адрес
        struct ifreq ifr_ip = interface.ifr[i];
        if (ioctl(temp_socket, SIOCGIFADDR, &ifr_ip) == 0) {
            struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr_ip.ifr_addr;
            std::string ip = inet_ntoa(ipaddr->sin_addr);
            _log.Info("- " + name + " (" + ip + ")\n");
        } else {
            _log.Info("- " + name + " (без IP)\n");
        }
    }

    close(temp_socket);
}