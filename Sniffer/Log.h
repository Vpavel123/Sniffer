#ifndef LOG_H
#define LOG_H

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
    void MessageBase(const std::string& log, const std::string& format);
    void Message(const std::string& log, const std::string& format);
    template<typename T, typename... Args>
    void Message(const std::string& log, const std::string& format, T value, Args... args);
    std::string getCurrentTime() const;
    std::string _text;
    FileLog* _file;
public:
    void Error(const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);

    template<typename... Args>
    void Error(const std::string& message, Args... args);
    template<typename... Args>
    void Info(const std::string& message, Args... args);
    template<typename... Args>
    void Warning(const std::string& message, Args... args);
    static Logger& getInstance() {
        static Logger instance; 
        return instance;
    }
    
    std::string GetText() const;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

template<typename T, typename... Args>
void Logger::Message(const std::string& log, const std::string& format, T value, Args... args) {
    size_t pos = format.find("{}");
    if (pos != std::string::npos) {
        std::ostringstream oss;
        oss << value;
        std::string new_format = format.substr(0, pos) + oss.str() + format.substr(pos + 2);
        Message(log, new_format, args...);
    } else {
        MessageBase(log, format);
    }

    _text = "[" + getCurrentTime() + "] " + log + " - " + format;
    _file->Init(_text);
}

inline void Logger::Message(const std::string& log, const std::string& format) {
        MessageBase(log, format);
}

template<typename... Args>
void Logger::Error(const std::string& message, Args... args){
    Message("Error", message, args...);
}

template<typename... Args>
void Logger::Info(const std::string& message, Args... args){
    Message("Info", message, args...);
}

template<typename... Args>
void Logger::Warning(const std::string& message, Args... args){
    Message("Warning", message, args...);
}

#endif // LOG_H