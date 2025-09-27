#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <iostream>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "PacketHandler.h"

#define BUFFER_SIZE 65536

class PacketProcessor
{
public:
    explicit PacketProcessor(PacketHandler& packetHandler) : _packetHandler(packetHandler) {}
    virtual ~PacketProcessor() = default;
    virtual void transmit(PacketHandler::IPVersion vers) = 0;
    virtual bool shouldProcessor(PacketHandler::IPVersion vers, PacketHandler::Protocol protocol) = 0;
protected:
    PacketHandler& _packetHandler;
};

class TCPProcessor : public PacketProcessor
{
public:
    explicit TCPProcessor(PacketHandler& packetHandler) : PacketProcessor(packetHandler) {}
    ~TCPProcessor() override = default;
    void transmit(PacketHandler::IPVersion vers) override;
    bool shouldProcessor(PacketHandler::IPVersion vers, PacketHandler::Protocol protocol) override;
};

class UPDProcessor : public PacketProcessor
{
public:
    explicit UPDProcessor(PacketHandler& packetHandler) : PacketProcessor(packetHandler) {}
    ~UPDProcessor() override = default;
    void transmit(PacketHandler::IPVersion vers) override;
    bool shouldProcessor(PacketHandler::IPVersion vers, PacketHandler::Protocol protocol) override;
};

class AllProtocolsProcessor : public PacketProcessor
{
public:
    explicit AllProtocolsProcessor(PacketHandler& packetHandler) : PacketProcessor(packetHandler) {}
    ~AllProtocolsProcessor() override = default;
    void transmit(PacketHandler::IPVersion vers) override;
    bool shouldProcessor(PacketHandler::IPVersion vers, PacketHandler::Protocol protocol) override;
};

class packetProcessorFactory{
public:
    static std::unique_ptr<PacketProcessor> processor(PacketHandler::Protocol protocol, PacketHandler& packetHandler);
};

#endif