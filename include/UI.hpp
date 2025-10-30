#include <ftxui/dom/elements.hpp>

#include "CPU.hpp"

#pragma once

class UI
{
public:
    ftxui::Element renderCPUCore(CPU::CPUCore core);
    ftxui::Element renderAllCPU(std::vector<CPU::CPUCore> cores);
};
