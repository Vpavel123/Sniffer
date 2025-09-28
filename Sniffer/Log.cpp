
#include "Log.h"

Logger::Logger() : _file(new FileLog("log.txt"))
{
    
}

void Logger::MessageBase(const std::string& log, const std::string& format) {
    std::cout << "[" << getCurrentTime() << "] " << log << " - " << format << std::endl;

    _text = "[" + getCurrentTime() + "] " + log + " - " + format;
    _file->Init(_text);
}

std::string Logger::GetText() const{
    return _text;
}

void Logger::Error(const std::string& message){
    MessageBase("Error", message);
}

void Logger::Info(const std::string& message){
    MessageBase("Info", message);
}

void Logger::Warning(const std::string& message){
    MessageBase("Warning", message);
}

std::string Logger::getCurrentTime() const{
    std::time_t now = std::time(nullptr);
    char buf[sizeof "2023-10-28 18:34:00"];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return buf;
} 