#include <ftxui/dom/elements.hpp>
#include <string>
#include <format>

#include "CPU.hpp"
#include "memory.hpp"

#pragma once

class UI
{
    private:
        std::vector<std::vector<ftxui::Element>> CPUGauges;   

        ftxui::Element renderCPUCore(CPU::CPUCore core, float memStressPercent);
        ftxui::Element renderHeader(CPU::Time uptime, CPU::Time idleTime, Memory::MemUsage memoryData);

    public:
        UI(int numCores);

        ftxui::Element renderAllCPU(std::vector<CPU::CPUCore> cores, CPU::Time uptime, CPU::Time idleTime, std::string cpuName, Memory::MemUsage memoryData, float memStressPercent);
};
