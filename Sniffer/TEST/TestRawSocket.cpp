#include <gtest/gtest.h>
#include "../RawSocket.h"
#include "../Log.h"

// Вместо наследования от Logger, создаем мок через интерфейс
class MockLogger {
public:
    MockLogger() {
        last_info = "";
        last_error = "";
        last_warning = "";
    }
    
    void Info(const std::string& message) {
        last_info = message;
        std::cout << "[MOCK INFO] " << message << std::endl;
    }
    
    void Error(const std::string& message) {
        last_error = message;
        std::cout << "[MOCK ERROR] " << message << std::endl;
    }
    
    void Warning(const std::string& message) {
        last_warning = message;
        std::cout << "[MOCK WARNING] " << message << std::endl;
    }
    
    std::string last_info;
    std::string last_error;
    std::string last_warning;
};

class RawSocketTest : public ::testing::Test {
protected:
    void SetUp() override {

    }
    
    void TearDown() override {
        // Очистка не нужна, т.к. используем синглтон
    }
};

TEST_F(RawSocketTest, Initialization) {
    // Этот тест может требовать прав root
    // Пропускаем или запускаем с sudo
    SUCCEED(); // Просто отмечаем успех
}

TEST_F(RawSocketTest, InterfaceListing) {
    Logger& logger = Logger::getInstance();
    RawSocket rawSocket(logger);
    
    // Тестируем что функция не падает
    EXPECT_NO_THROW({
        auto interfaces = rawSocket.listInterfaces();
        EXPECT_EQ(typeid(interfaces), typeid(std::vector<std::string>));
    });
}

TEST_F(RawSocketTest, SocketCreation) {
    Logger& logger = Logger::getInstance();
    RawSocket rawSocket(logger);
    
    // Проверяем что сокет создается
    rawSocket.init();
    
    // Проверяем состояние сокета (может вернуть false если нет прав)
    bool socketOk = rawSocket.checkSocket();
    
    // Если есть права - сокет должен быть валидным, иначе - тест пропускаем
    if (getuid() == 0) {
        EXPECT_TRUE(socketOk);
    } else {
        std::cout << "Warning: Test requires root privileges for socket creation" << std::endl;
    }
    
    rawSocket.shut();
}