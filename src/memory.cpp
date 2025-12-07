#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "memory.hpp"

Memory::Memory()
{
    // TODO - may need to explicitly set lastMemInfo and lastVmStatInfo
    // and maybe calculate memoryData from here initially
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
        std::istringstream iss(line);
        std::string label;
        unsigned long long value;
        std::string unit;
        
        if (line.rfind("MemTotal:", 0) == 0) {
            iss >> label >> value >> unit;
            memInfo.total = value;
        }
        else if (line.rfind("MemAvailable:", 0) == 0) {
            iss >> label >> value >> unit;
            memInfo.available = value;
        }
        else if (line.rfind("Cached:", 0) == 0) {
            iss >> label >> value >> unit;
            memInfo.cached = value;
        }
        else if (line.rfind("Active:", 0) == 0) {
            iss >> label >> value >> unit;
            memInfo.active = value;
        }
        else if (line.rfind("Dirty:", 0) == 0) {
            iss >> label >> value >> unit;
            memInfo.dirty = value;
        }
        else if (line.rfind("Writeback:", 0) == 0) {
            iss >> label >> value >> unit;
            memInfo.writeback = value;
        }
    }

    return memInfo;
}

Memory::VmStat Memory::parseVmStat()
{
    std::ifstream file("/proc/vmstat");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open /proc/vmstat");
    }

    std::string line;
    VmStat vmStat;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string label;
        unsigned long long value;
        
        if (line.rfind("pgfault", 0) == 0) {
            iss >> label >> value;
            vmStat.pgFault = value;
        }
        else if (line.rfind("pgmajfault", 0) == 0) {
            iss >> label >> value;
            vmStat.pgMajFault = value;
        }
        else if (line.rfind("pswpin", 0) == 0) {
            iss >> label >> value;
            vmStat.pSwpIn = value;
        }
        else if (line.rfind("pswpout", 0) == 0) {
            iss >> label >> value;
            vmStat.pSwpOut = value;
        }
    }

    return vmStat;
}

void Memory::memoryUpdate()
{
    VmStat curVmStatInfo = parseVmStat();
    MemInfo curMemInfo = parseMemory();
    unsigned long long total = curMemInfo.total;
    unsigned long long available = curMemInfo.available;

    int pgMajFaultDelta = static_cast<int>(curVmStatInfo.pgMajFault - lastVmStatInfo.pgMajFault);
    int pgFaultDelta = static_cast<int>(curVmStatInfo.pgFault - lastVmStatInfo.pgFault);
    int pSwpInDelta = static_cast<int>(curVmStatInfo.pSwpIn - lastVmStatInfo.pSwpIn);
    int pSwpOutDelta = static_cast<int>(curVmStatInfo.pSwpOut - lastVmStatInfo.pSwpOut);
    int writebackDelta = static_cast<int>(curMemInfo.writeback - lastMemInfo.writeback);

    int normalMajFault = std::clamp(pgMajFaultDelta / MAX_PGMAJFAULT, 0, 1);
    int normalFault = std::clamp(pgFaultDelta / MAX_PGFAULT, 0, 1);
    int normalSwpIn = std::clamp(pSwpInDelta / MAX_PSWP, 0, 1);
    int normalSwpOut = std::clamp(pSwpOutDelta / MAX_PSWP, 0, 1);
    int normalSwp = (normalSwpIn + normalSwpOut) / 2;

    int normalAvailable = std::clamp(1 - (static_cast<int>(available / total)), 0, 1);
    int normalDirty = std::clamp(static_cast<int>(curMemInfo.dirty / total), 0, 1);
    int normalWriteback = std::clamp(static_cast<int>(writebackDelta / MAX_WRITEBACK), 0, 1);
    int normalCached = std::clamp(1 - (static_cast<int>(curMemInfo.cached / total)), 0, 1);
    int normalActive = std::clamp(static_cast<int>(curMemInfo.active / total), 0, 1);

    float memStressRawVal = W_PGMAJFAULT * normalMajFault +
        W_PGFAULT * normalFault +
        W_PSWP * normalSwp +
        W_AVAILABLE * normalAvailable + 
        W_DIRTY * normalDirty + 
        W_WRITEBACK * normalWriteback +
        W_CACHED * normalCached +
        W_ACTIVE * normalActive;
    
    // TODO - consider applying smoothing to this value
    memStressPercent = std::round(std::clamp(memStressRawVal, 0.0f, 1.0f) * 100.0f);
    
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

    // set last to current in preparation for next update
    lastMemInfo = curMemInfo;
    lastVmStatInfo = curVmStatInfo;
}
