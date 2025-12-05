#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#pragma once

class Memory
{
    public:
        Memory();

        struct MemUsage
        {
            unsigned long long total = 0;
            unsigned long long usage = 0;
        };

        struct MemInfo
        {
            unsigned long long total = 0;
            unsigned long long available = 0;
        };

        MemUsage memoryData;
        void memoryUpdate();

    private:
        MemInfo parseMemory();
};
