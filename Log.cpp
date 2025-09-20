
#include "Log.h"

Logger::Logger() 
{
    
}

std::string Logger::GetText(){
    return _text;
}

void Logger::Message(std::string log, std::string message){
    std::cout << "[" << getCurrentTime() << "] " << log << " - " << message << std::endl;
    std::string log_text = "[" + getCurrentTime() + "] " + log + " - " + message;

    
    std::thread stream_text([this, log_text]() { 
        mtx.lock();
            _text = log_text;
            _file->Init(_text);
        mtx.unlock();
    });

    stream_text.join();
}

void Logger::Error(std::string message){
    Message("Error", message);
}

void Logger::Info(std::string message){
    Message("Info", message);
}

void Logger::Warning(std::string message){
    Message("Warning", message);
}

std::string Logger::getCurrentTime(){
    std::time_t now = std::time(nullptr);
    char buf[sizeof "2023-10-28 18:34:00"];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return buf;
} 