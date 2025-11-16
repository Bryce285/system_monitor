#include <ftxui/dom/elements.hpp>
#include <string>
#include <format>

#include "UI.hpp"
#include "CPU.hpp"

ftxui::Element UI::renderCPUCore(CPU::CPUCore core)
{
    float coreUsage = static_cast<float>(core.usagePercent) / 100.0f;
    float usageDisplay = coreUsage * 100.0f;
    coreUsage = std::clamp(coreUsage, 0.0f, 1.0f);

    return  ftxui::hbox({
	    ftxui::text(core.id) | ftxui::bold | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 8),
	    ftxui::separator(),

	    ftxui::gauge(coreUsage) | ftxui::flex,
	    ftxui::separator(),

	    ftxui::text(std::to_string(usageDisplay).substr(0, 5) + "%") | ftxui::center
    });
}

ftxui::Element UI::renderUptime(CPU::Time uptime, CPU::Time idleTime)
{
    std::string uptimeStr = std::format("{:02}:{:02}:{:02}:{:02}", uptime.days, uptime.hours, uptime.minutes, uptime.seconds);

    std::string idleTimeStr = std::format("{:02}:{:02}:{:02}:{:02}", idleTime.days, idleTime.hours, idleTime.minutes, idleTime.seconds);

    return  ftxui::hbox({
	    ftxui::text("System up time: " + uptimeStr),
	    ftxui::filler(),
	    ftxui::text("Time in idle process: " + idleTimeStr)
	    });
}

ftxui::Element UI::renderAllCPU(std::vector<CPU::CPUCore> cores, CPU::Time uptime, CPU::Time idleTime)
{
    std::vector<ftxui::Element> CPUGauges;
    CPUGauges.resize(cores.size());

    for (size_t i = 0; i < cores.size(); ++i) {
	CPUGauges[i] = renderCPUCore(cores[i]);
    }

    auto gaugesElement = ftxui::vbox({ 
	    ftxui::vbox(CPUGauges)
	    }) | ftxui::border;

    auto uptimeElement = renderUptime(uptime, idleTime);

    auto document = ftxui::window(ftxui::text("CPU") | ftxui::hcenter | ftxui::bold,
	    ftxui::vbox({
	    uptimeElement,
	    gaugesElement
	    }));

    return document;
}
