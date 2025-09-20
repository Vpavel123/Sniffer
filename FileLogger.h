#include <fstream>

class FileLog{
public:
    FileLog(std::string path);
    ~FileLog();

    std::string GetPath();
    void Init(std::string text);
private:
    std::string _path;
    std::ofstream out;
};
