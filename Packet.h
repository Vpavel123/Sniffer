#include <iostream>

#define BUFFER_SIZE 65536

class Packet
{
private:
    unsigned char *buffer;
public:
    Packet();
    ~Packet();
};