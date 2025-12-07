#include <ftxui/dom/elements.hpp>
//#include <ftxui/util/color.hpp>
#include <string>
#include <format>

#include "UI.hpp"
#include "CPU.hpp"
#include "memory.hpp"

UI::UI(int numCores)
{
    // temporary CPUCore object for initialization
    CPU::CPUCore tempCore;

    // 2d vector to store cpu util % gauges from the last 20 refreshes for each core
    UI::CPUGauges.resize(numCores);
    int numGauges = 10;
    for (size_t i = 0; i < UI::CPUGauges.size(); ++i) {
        UI::CPUGauges[i].resize(numGauges);
        UI::CPUGauges[i].assign(numGauges, renderCPUCore(tempCore, 0.0f));
    }
}

ftxui::Element UI::renderCPUCore(CPU::CPUCore core, float memStressPercent)
{
    float coreUsage = static_cast<float>(core.usagePercent) / 100.0f;
    float usageDisplay = coreUsage * 100.0f;
    coreUsage = std::clamp(coreUsage, 0.0f, 1.0f);

    int red = 0;
    int green = 0;
    int blue = 0;
    if (memStressPercent <= 25.0f) {
        // green
        red = 63;
        green = 142;
        blue = 79;
    }
    else if (memStressPercent > 25.0f && memStressPercent <= 50.0f) {
        // yellow
        red = 245;
        green = 203;
        blue = 41;
    }
    else if (memStressPercent > 50.0f && memStressPercent <= 75.0f) {
        // orange
        red = 218;
        green = 110;
        blue = 24;
    }
    else if (memStressPercent > 75.0f && memStressPercent <= 100.0f) {
        // red
        red = 186;
        green = 64;
        blue = 64;
    }
    else {
        // grey
        // this indicates something went wrong calculating stressPercent
        red = 36;
        green = 31;
        blue = 30;
    }

    return  ftxui::vbox({
            ftxui::gaugeUp(coreUsage) | ftxui::bgcolor(ftxui::Color::RGB(red, green, blue)) | ftxui::flex
            });
}

ftxui::Element UI::renderHeader(CPU::Time uptime, CPU::Time idleTime, Memory::MemUsage memoryData)
{
    std::string uptimeStr = std::format("{:03}:{:02}:{:02}:{:02}", uptime.days, uptime.hours, uptime.minutes, uptime.seconds);

    std::string idleTimeStr = std::format("{:03}:{:02}:{:02}:{:02}", idleTime.days, idleTime.hours, idleTime.minutes, idleTime.seconds);

    std::string totalStr = std::to_string(memoryData.total);
    std::string usageStr = std::to_string(memoryData.usage);

    return  ftxui::hbox({
            ftxui::text("System up time: " + uptimeStr),
            ftxui::filler(),
            ftxui::text("Memory Usage: [ " + usageStr + " / " + totalStr + " ] kB"),
            ftxui::filler(),
            ftxui::text("Time in idle process: " + idleTimeStr)
            });
}

ftxui::Element UI::renderAllCPU(std::vector<CPU::CPUCore> cores, CPU::Time uptime, CPU::Time idleTime, std::string cpuName, Memory::MemUsage memoryData, float memStressPercent)
{
    UI::CPUGauges.resize(cores.size());

    // this vector will hold the graph element that will be created from each
    // nested vector in CPUGauges
    std::vector<ftxui::Element> graphs;
    int minGraphHeight = 15;

    for (size_t i = 0; i < cores.size(); ++i) {
        CPU::CPUCore curCore = cores[i];
        UI::CPUGauges[i].push_back(renderCPUCore(curCore, memStressPercent));
        UI::CPUGauges[i].erase(UI::CPUGauges[i].begin());

        auto utilGraph = ftxui::vbox({
                ftxui::hbox({
                        ftxui::text(curCore.id) | ftxui::center
                        }),
                
                // TODO - show the background color somewhere else near the gauge as well so
                // that even when the cpu is at 100% utilization the memory info is still visible
                ftxui::hbox(UI::CPUGauges[i])
                | ftxui::border
                | ftxui::size(ftxui::HEIGHT, ftxui::GREATER_THAN, minGraphHeight)
                | ftxui::yflex,

                ftxui::hbox({
                        ftxui::text(std::to_string(curCore.usagePercent).substr(0, 5) + "%")
                        | ftxui::center
                        })
                }) | ftxui::yflex;

        graphs.push_back(utilGraph);
    }

    std::vector<ftxui::Element> spacedGraphs;
    for (size_t i = 0; i < graphs.size(); ++i) {
        spacedGraphs.push_back(graphs[i]);
        if (i != graphs.size() - 1) {
            spacedGraphs.push_back(ftxui::filler());
        }
    }

    auto allGraphs = ftxui::hbox(spacedGraphs) | ftxui::xflex;

    auto header = renderHeader(uptime, idleTime, memoryData);
    
    auto document = ftxui::window(ftxui::text(cpuName) | ftxui::bold,
            ftxui::vbox({
                header,
                ftxui::separator(),
                allGraphs | ftxui::yflex
                }) | ftxui::yflex);

    return document;
}
