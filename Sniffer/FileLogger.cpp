#include "FileLogger.h"

FileLog::FileLog(std::string path) : _path(path){
    out.open(_path, std::ios::app);
    if (!out.is_open()) {
        std::cerr << "Cannot open log file: " << _path << std::endl;
    }
}

FileLog::~FileLog(){
    out.clear();
    out.close();
}

std::string FileLog::GetPath(){
    return this->_path;
}

void FileLog::Init(std::string text) {
    std::lock_guard<std::mutex> lock(_mutex);  // Синхронизация
    
    if (out.is_open()) {
        out << text << std::endl;
        out.flush();  // Принудительно сбрасываем буфер
    } else {
        std::cout << "Log file is not open! Trying to reopen..." << std::endl;
        out.open(_path, std::ios::app);
        if (out.is_open()) {
            out << text << std::endl;
            out.flush();
        }
    }
}