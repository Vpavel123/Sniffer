Network Packet Sniffer
Простой сетевой сниффер на C++ для захвата и анализа сетевых пакетов.

Возможности
Захват пакетов в реальном времени
Поддержка протоколов: TCP, UDP, IPv4, IPv6
Фильтрация по типу протокола и IP версии
Выбор сетевого интерфейса
Логирование в файл и консоль
Интуитивное меню для управления

Требования
Системные требования
Linux (тестировалось на Ubuntu/Debian)
Права root для захвата пакетов
Компилятор C++ с поддержкой C++17

Зависимости
CMake (версия 3.14 или выше)
Google Test (для unit тестов, опционально)

Установка и сборка
1. Клонирование репозитория
bash
git clone https://github.com/Vpavel123/Sniffer
cd Sniffer
2. Сборка проекта
bash
# Создание директории для сборки
mkdir build
cd build

# Генерация Makefile
cmake ..

# Компиляция
make
3. Запуск с правами root
bash
sudo ./network_sniffer
Использование
Главное меню
text
===========================
 MAIN MENU 
===========================
1. Start
2. Configure Interface
3. Select IP Type (IPv4/IPv6)
4. Select Protocol (TCP/UDP)
5. Exit
Enter your choice:
Опции меню
Start - Запуск захвата пакетов (максимум 10 пакетов для демонстрации)
Configure Interface - Выбор сетевого интерфейса
Select IP Type - Фильтрация по версии IP:
IPv4
IPv6
ALL (все версии)
Select Protocol - Фильтрация по протоколу:
TCP
UDP
ALL (все протоколы)
Exit - Выход из программы
Пример вывода пакета
text
=== TCP Packet (IPv4) ===
Source Port: 443
Destination Port: 54321
Source IP: 192.168.1.1
Destination IP: 192.168.1.100
Тестирование
Запуск unit тестов
bash
cd build
./run_test
Структура тестов
TestRawSocket.cpp - тесты для RawSocket класса
TestPacketHandler.cpp - тесты для обработки пакетов
TestPacketProcessor.cpp - тесты для процессоров пакетов
Структура проекта
text
Sniffer/
├── CMakeLists.txt          # Конфигурация сборки                    
# Исходные файлы
├── main.cpp            # Главная функция
├── RawSocket.cpp       # Работа с raw socket
├── PacketHandler.cpp   # Обработка пакетов
├── PacketProcessor.cpp # Анализ протоколов
├── ConsoleState.cpp    # Управление меню
└── Log.cpp             # Система логирования
# Заголовочные файлы
├── RawSocket.h
├── PacketHandler.h
├── PacketProcessor.h
├── ConsoleState.h
└── Log.h
├── TEST/                  # Unit тесты
│   ├── TestRawSocket.cpp
│   ├── TestPacketHandler.cpp
│   └── TestPacketProcessor.cpp
# Директория сборки
🏗 Архитектура
Основные классы
RawSocket - управление raw socket и сетевыми интерфейсами
PacketHandler - парсинг и обработка сетевых пакетов
PacketProcessor - анализ конкретных протоколов (TCP/UDP)
Logger - система логирования (синглтон)
Menu - управление пользовательским интерфейсом
Паттерны проектирования
Фабрика - PacketProcessorFactory для создания процессоров
Стратегия - разные процессоры для разных протоколов
Синглтон - Logger для глобального доступа к логам

Важные замечания
Безопасность
Требуются права root для создания raw socket
Используйте только в тестовых целях
Соблюдайте местные законы о перехвате сетевого трафика

Ограничения
Захватывает только первые 10 пакетов в демо-режиме
Ограниченная поддержка IPv6 расширений
Базовый анализ транспортного уровня
