cmake_minimum_required(VERSION 3.14)
project(NetworkSnifferTests)

# Находим GoogleTest
find_package(GTest REQUIRED)

# Исходные файлы проекта
set(SOURCES
    RawSocket.cpp
    PacketHandler.cpp
    PacketProcessor.cpp
    ConsoleState.h
    FileLogger.cpp
    Log.cpp
)

# Файлы тестов
set(TEST_SOURCES
    TEST/unit_test.cpp
    TEST/TestRawSocket.cpp
    TEST/TestPacketHandler.cpp
    TEST/TestPacketProcessor.cpp
)

# Создаем исполняемый файл для тестов
add_executable(run_tests ${TEST_SOURCES} ${SOURCES})

# Подключаем зависимости
target_include_directories(run_tests PRIVATE include)
target_link_libraries(run_tests GTest::gtest GTest::gtest_main pthread)

# Добавляем target для запуска тестов
add_test(NAME AllTests COMMAND run_tests)
