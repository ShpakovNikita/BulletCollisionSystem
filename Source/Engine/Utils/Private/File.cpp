#include "Utils/File.hpp"
#include <fstream>

std::string File::ReadFile(const std::string& filePath)
{
    constexpr size_t readSize = 4096;
    std::ifstream stream(filePath);
    stream.exceptions(std::ios_base::badbit);

    std::string out;
    std::string buf = std::string(readSize, '\0');
    while (stream.read(&buf[0], readSize)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}
