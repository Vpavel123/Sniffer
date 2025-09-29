#ifndef FILELOG_H
#define FILELOG_H

#include <iostream>
#include <fstream>
#include <mutex>

class FileLog{
public:
    FileLog(std::string path);
    ~FileLog();

    std::string GetPath();
    void Init(std::string text);
private:
    std::string _path;
    std::ofstream out;
    std::mutex _mutex;
};

#endif // FILELOG_H
