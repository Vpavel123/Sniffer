#include <iostream>
#include <memory>
#include <stdlib.h>

#include "RawSocket.h"
#include "PacketHandler.h"
#include "PacketProcessor.h"

class MenuAction {
public:
    MenuAction(RawSocket& rawSocket, PacketHandler& handler,
               PacketHandler::IPVersion& ipVersion,
               PacketHandler::Protocol& protocol)
        : _rawSocket(rawSocket), _handler(handler),
          _ipVersion(ipVersion), _protocol(protocol) {}

    virtual void execute() = 0;
    virtual ~MenuAction() = default;

protected:
    RawSocket& _rawSocket;
    PacketHandler& _handler;
    PacketHandler::IPVersion& _ipVersion;
    PacketHandler::Protocol& _protocol;
};

// Действие для старта
class StartAction : public MenuAction {
public:
    using MenuAction::MenuAction;

    void execute() override {
        if (!_rawSocket.checkSocket())
        {
            _rawSocket.shut();
        }
        
        std::cout << "System started. Waiting for packets...\n";
        std::cout << "Selected IP version: " << _ipVersion << "\n";
        std::cout << "Selected protocol: " << _protocol << "\n";

        auto processor = PacketProcessorFactory::createProcessor(_protocol, _handler);

        int packet_count = 0;
        while (packet_count < 10) {  // Ограничим для теста
            ssize_t data_size = _rawSocket.recieve(_handler.getBuffer(), BUFFER_SIZE, 0);
            if (data_size < 0) {
                _rawSocket._log.Error("Recvfrom error");
                break;
            }

            std::cout << "\n=== Packet " << ++packet_count << " ===" << std::endl;
            std::cout << "Raw data size: " << data_size << " bytes" << std::endl;

            auto parsed_packet = _handler.parseBuffer(_handler.getBuffer(), data_size);

            // Простая проверка - выводим все пакеты для отладки
            std::cout << "[DEBUG] Packet IP version: " << (int)parsed_packet.ip_version 
                      << ", Selected IP version: " << _ipVersion << std::endl;
            
            if (parsed_packet.ip_version == 4 || parsed_packet.ip_version == 6) {
                processor->transmit(_ipVersion);
            } else {
                _rawSocket._log.Warning("Non-IP packet skipped");
            }
            
            usleep(100000); // Пауза 100ms между пакетами для читаемости
        }
        
        std::cout << "Finished capturing 10 packets. Returning to menu.\n";
    }
};

// Действие для настройки интерфейса
class InterfaceAction : public MenuAction
{
public:
    using MenuAction::MenuAction;
    void execute() override
    {
        std::vector<std::string> _vector = _rawSocket.listInterfaces();

        std::cout << "Choose interface:\n";
        int choice;
        
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        if (choice == 0){
            std::cout << "Invalid choice\n";
        }else{
            _rawSocket.bindInterface(_vector[choice - 1]);
        }
    }
private:
    std::string name;
};

// Действие для выбора типа IP (IPv4, IPv6)
class IPTypeAction : public MenuAction {
public:
    using MenuAction::MenuAction;

    void execute() override {
        std::cout << "Select IP type:\n";
        std::cout << "1. IPv4\n";
        std::cout << "2. IPv6\n";
        std::cout << "3. ALL\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                _ipVersion = PacketHandler::IPv4;
                break;
            case 2:
                _ipVersion = PacketHandler::IPv6;
                break;
            case 3:
                _ipVersion = PacketHandler::ANY;
                break;
            default:
                std::cout << "Invalid choice\n";
        }
    }
};

// Действие для выбора протокола (TCP, UDP)
class ProtocolAction : public MenuAction {
public:
    using MenuAction::MenuAction;

    void execute() override {
        std::cout << "Select protocol:\n";
        std::cout << "1. TCP\n";
        std::cout << "2. UDP\n";
        std::cout << "3. ALL\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                _protocol = PacketHandler::TCP;
                std::cout << "TCP selected\n";
                break;
            case 2:
                _protocol = PacketHandler::UDP;
                std::cout << "UDP selected\n";
                break;
            case 3:
                _protocol = PacketHandler::ALL;
                std::cout << "ALL selected\n";
                break;
            default:
                std::cout << "Invalid choice\n";
        }
    }
};

// Действие для закрытия
class ExitAction : public MenuAction
{
public:
    using MenuAction::MenuAction;
    void execute() override
    {
        std::cout << "Exiting the program...\n";
        exit(0);
    }
};
 // Создания объектов действий
class MenuActionFactory {
public:
    std::unique_ptr<MenuAction> createAction(
        RawSocket& rawSocket,
        PacketHandler& handler,
        int choice,
        PacketHandler::IPVersion& ipVersion,
        PacketHandler::Protocol& protocol)
    {
        switch (choice) {
            case 1: return std::make_unique<StartAction>(rawSocket, handler, ipVersion, protocol);
            case 2: return std::make_unique<InterfaceAction>(rawSocket, handler, ipVersion, protocol);
            case 3: return std::make_unique<IPTypeAction>(rawSocket, handler, ipVersion, protocol);
            case 4: return std::make_unique<ProtocolAction>(rawSocket, handler, ipVersion, protocol);
            case 5: return std::make_unique<ExitAction>(rawSocket, handler, ipVersion, protocol);
            default:
                std::cout << "Invalid choice\n";
                return nullptr;
        }
    }
};


/* Само меню */
class Menu {
public:
    Menu() : factory(std::make_unique<MenuActionFactory>()),
             selectedIPVersion(PacketHandler::ANY),
             selectedProtocol(PacketHandler::ALL) {}

    void displayMainMenu() const {
        std::cout << "===========================\n";
        std::cout << " MAIN MENU \n";
        std::cout << "===========================\n";
        std::cout << "1. Start\n";
        std::cout << "2. Configure Interface\n";
        std::cout << "3. Select IP Type (IPv4/IPv6)\n";
        std::cout << "4. Select Protocol (TCP/UDP)\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
    }

    void handleChoice(RawSocket& rawSocket, PacketHandler& handler) {
        int choice;
        while (true) {
            displayMainMenu();
            std::cin >> choice;
            auto action = factory->createAction(rawSocket, handler, choice, selectedIPVersion, selectedProtocol);
            if (action) {
                action->execute();
                if (choice == 5) {
                    break;
                }
            }
        }
    }

private:
    std::unique_ptr<MenuActionFactory> factory;
    PacketHandler::IPVersion selectedIPVersion;
    PacketHandler::Protocol selectedProtocol;
};
