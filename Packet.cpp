#include "Packet.h"

Packet::Packet(){
    buffer = new unsigned char[BUFFER_SIZE];
}

Packet::~Packet(){
    delete[] buffer;
}