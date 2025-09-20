#include "FileLogger.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <ctime>
#include <thread>
#include <mutex>

class Logger
{
private:
    Logger();
    void Message(std::string log, std::string message);
    std::string getCurrentTime();
    std::string _text;
    FileLog* _file = new FileLog("log.txt");
public:
    void Error(std::string message);
    void Info(std::string message);
    void Warning(std::string message);
    static Logger& getInstance() {
        static Logger instance; 
        return instance;
    }
    
    std::mutex mtx;
    std::string GetText();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};