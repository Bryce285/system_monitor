#include <ftxui/dom/elements.hpp>
#include <string>
#include <format>

#include "CPU.hpp"

#pragma once

class UI
{
private:
    std::vector<std::vector<ftxui::Element>> CPUGauges;   

    ftxui::Element renderCPUCore(CPU::CPUCore core);
    ftxui::Element renderUptime(CPU::Time uptime, CPU::Time idleTime);

public:
    UI(int numCores);

    ftxui::Element renderAllCPU(std::vector<CPU::CPUCore> cores, CPU::Time uptime, CPU::Time idleTime);
};
