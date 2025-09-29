#include <gtest/gtest.h>
#include "../PacketProcessor.h"
#include "../PacketHandler.h"

class PacketProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        handler = new PacketHandler();
    }
    
    void TearDown() override {
        delete handler;
    }
    
    PacketHandler* handler;
};

TEST_F(PacketProcessorTest, FactoryCreation) {
    // Тестируем фабрику процессоров
    auto tcpProcessor = PacketProcessorFactory::createProcessor(
        PacketHandler::TCP, *handler);
    EXPECT_NE(tcpProcessor, nullptr);
    
    auto udpProcessor = PacketProcessorFactory::createProcessor(
        PacketHandler::UDP, *handler);
    EXPECT_NE(udpProcessor, nullptr);
    
    auto allProcessor = PacketProcessorFactory::createProcessor(
        PacketHandler::ALL, *handler);
    EXPECT_NE(allProcessor, nullptr);
}

TEST_F(PacketProcessorTest, DefaultProcessor) {
    // Тестируем создание процессора по умолчанию
    auto processor = PacketProcessorFactory::createProcessor(
        static_cast<PacketHandler::Protocol>(999), *handler); // Неизвестный протокол
    EXPECT_NE(processor, nullptr);
}
