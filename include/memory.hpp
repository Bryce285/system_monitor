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
            unsigned long long dirty = 0;
            unsigned long long writeback = 0;
            unsigned long long cached = 0;
            unsigned long long active = 0;
        };

        struct VmStat
        {
            unsigned long long pgFault = 0;
            unsigned long long pgMajFault = 0;
            unsigned long long pSwpIn = 0;
            unsigned long long pSwpOut = 0;
        };

        MemUsage memoryData;
        MemInfo lastMemInfo;
        VmStat lastVmStatInfo;
        float memStressPercent = 0.0f;

        void memoryUpdate();

    private:
        // max rates for normalization
        const int MAX_PGMAJFAULT = 20;
        const int MAX_PGFAULT = 2000;
        const int MAX_PSWP = 10;
        const int MAX_WRITEBACK = 50;
        
        // weights for calculating memory stress value
        const float W_PGMAJFAULT = 0.28f;
        const float W_PGFAULT = 0.07f;
        const float W_PSWP = 0.23f;
        const float W_AVAILABLE = 0.20f;
        const float W_DIRTY = 0.06f;
        const float W_WRITEBACK = 0.06f;
        const float W_CACHED = 0.05f;
        const float W_ACTIVE = 0.05f;

        MemInfo parseMemory();
        VmStat parseVmStat();
};
