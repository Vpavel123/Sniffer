#include <iostream>
#include "Packet.h"

class ConsoleState
{
public:
    virtual void display(char choice) = 0;
    virtual void ConsoleHandle() = 0;
    virtual ~ConsoleState() = default;
};

class MenuConsole : public ConsoleState
{
public:
    void display(char choice) override;
    void ConsoleHandle() override;
private:
    PacketHandler& packet;
};

void MenuConsole::display(char choice){
    std::cout << "================"
    << "     MENU      \n        =================        \n"  
    << "1. Start" << std::endl; 

    switch (choice)
    {
    case '1':
            packet.FilterProtocol(TCP);
        break;
    
    default:
        break;
    }
}