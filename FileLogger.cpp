#include "FileLogger.h"

FileLog::FileLog(std::string path) : _path(path){
}

FileLog::~FileLog(){
    out.clear();
    out.close();
}

std::string FileLog::GetPath(){
    return this->_path;
}

void FileLog::Init(std::string text){
    out.open(_path, std::ios::app);
    if (out.is_open())
    {
        out << text << std::endl;
    }
    
}