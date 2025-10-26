#include <ftxui/dom/elements.hpp>

#include "UI.hpp"
#include "CPU.hpp"

ftxui::Element UI::renderCPUCore(CPU::CPUCore core)
{
    float coreUsage = static_cast<float>(core.usagePercent) / 100.0f;
    float usageDisplay = coreUsage * 100.0f;
    coreUsage = std::clamp(coreUsage, 0.0f, 1.0f);

    return  ftxui::vbox({
	    ftxui::text("CPU") | ftxui::bold | ftxui::center,
	    ftxui::separator(),
	    ftxui::gauge(coreUsage) | ftxui::flex,
	    ftxui::text(std::to_string(usageDisplay).substr(0, 5) + "%") | ftxui::center
    }) | ftxui::border;
}
