#include "RawSocket.h"

RawSocket::RawSocket(){
    saddr_len = sizeof(saddr);
}

RawSocket::~RawSocket(){
    Close();
}

void RawSocket::Socket(){
    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_socket < 0) {
        //perror("Socket error");
        log.Error("Socket error");
    }
}

void RawSocket::Close(){
    if (raw_socket != -1)
    {
        close(raw_socket);
        raw_socket = -1;
    }
    
} 