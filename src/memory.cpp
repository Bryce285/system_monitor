#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include "memory.hpp"

Memory::Memory()
{
    memoryUpdate();
}

Memory::MemInfo Memory::parseMemory()
{
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open /proc/meminfo");
    }

    std::string line;
    MemInfo memInfo;

    while (std::getline(file, line)) {
        if (line.rfind("MemTotal:", 0) == 0){
            std::istringstream iss(line);

            std::string label;
            unsigned long long value;
            std::string unit;

            iss >> label >> value >> unit;
            memInfo.total = value;
        }
        else if (line.rfind("MemAvailable:", 0) == 0){
            std::istringstream iss(line);

            std::string label;
            unsigned long long value;
            std::string unit;

            iss >> label >> value >> unit;
            memInfo.available = value;
        }
    }

    return memInfo;
}

void Memory::memoryUpdate()
{
    MemInfo memInfo = parseMemory();
    unsigned long long total = memInfo.total;
    unsigned long long available = memInfo.available;

    if (total == 0 || total < available) {
        memoryData.total = 0;
        memoryData.usage = 0;
        return;
    } 
    else {
        unsigned long long usage = total - available;
        memoryData.total = total;
        memoryData.usage = usage;
    }
}
