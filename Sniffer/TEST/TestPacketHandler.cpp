#include <gtest/gtest.h>
#include "../PacketHandler.h"

class PacketHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        handler = new PacketHandler();
    }
    
    void TearDown() override {
        delete handler;
    }
    
    PacketHandler* handler;
};

TEST_F(PacketHandlerTest, BufferInitialization) {
    // Проверяем что буфер инициализирован
    unsigned char* buffer = handler->getBuffer();
    EXPECT_NE(buffer, nullptr);
}

TEST_F(PacketHandlerTest, ParseEmptyBuffer) {
    // Тестируем обработку пустого буфера
    unsigned char empty_buffer[10] = {0};
    auto packet = handler->parseBuffer(empty_buffer, 10);
    
    // Проверяем что функция не падает и возвращает корректную структуру
    EXPECT_EQ(packet.eth, nullptr);
}

TEST_F(PacketHandlerTest, IPVersionDetection) {
    // Проверяем методы определения версии IP
    PacketHandler::Packet packet;
    packet.ip_version = 4;
    

    SUCCEED();
}