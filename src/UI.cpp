#include <ftxui/dom/elements.hpp>
#include <string>
#include <format>

#include "UI.hpp"
#include "CPU.hpp"

UI::UI(int numCores)
{
    // temporary CPUCore object for initialization
    CPU::CPUCore tempCore;

    // 2d vector to store cpu util % gauges from the last 20 refreshes for each core
    // TODO - 20 gauges cant all fit on a single graph which is causing a delay
    // in the data being displayed
    UI::CPUGauges.resize(numCores);
    for (size_t i = 0; i < UI::CPUGauges.size(); ++i) {
	UI::CPUGauges[i].resize(20);
	UI::CPUGauges[i].assign(20, renderCPUCore(tempCore));
    }
}

ftxui::Element UI::renderCPUCore(CPU::CPUCore core)
{
    float coreUsage = static_cast<float>(core.usagePercent) / 100.0f;
    float usageDisplay = coreUsage * 100.0f;
    coreUsage = std::clamp(coreUsage, 0.0f, 1.0f);

    return  ftxui::vbox({
	    ftxui::gaugeUp(coreUsage) | ftxui::flex
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
    UI::CPUGauges.resize(cores.size());

    // this vector will hold the graph element that will be created from each
    // nested vector in CPUGauges
    std::vector<ftxui::Element> graphs;

    for (size_t i = 0; i < cores.size(); ++i) {
	UI::CPUGauges[i].push_back(renderCPUCore(cores[i]));
	UI::CPUGauges[i].erase(UI::CPUGauges[i].begin());
	
	// TODO - include core labels and util % on graphs
	auto utilGraph = ftxui::hbox(UI::CPUGauges[i]) 
	    | ftxui::border
	    | ftxui::size(ftxui::HEIGHT, ftxui::GREATER_THAN, 15);
	graphs.push_back(utilGraph);
    }

    auto allGraphs = ftxui::hbox(graphs);

    auto uptimeElement = renderUptime(uptime, idleTime);

    auto document = ftxui::window(ftxui::text("CPU") | ftxui::hcenter | ftxui::bold,
	    ftxui::vbox({
	    uptimeElement,
	    allGraphs
	    }));

    return document;
}
