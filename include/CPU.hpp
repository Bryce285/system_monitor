#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#pragma once

class CPU
{
public:
    CPU();

    struct CPUCore
    {
	std::string id;
	unsigned long long user = 0;
	unsigned long long nice = 0;
	unsigned long long system = 0; 
	unsigned long long idle = 0; 
	unsigned long long iowait = 0; 
	unsigned long long irq = 0; 
	unsigned long long softirq = 0; 
	unsigned long long steal = 0; 
	unsigned long long guest = 0; 
	unsigned long long guest_nice = 0;

	double usagePercent = 0.0;
    };

    std::vector<CPUCore> parseCores();

    unsigned long long getIdle(CPUCore core);
    unsigned long long getTotal(CPUCore core);
    double getUsagePercent(unsigned long long idlePrev, unsigned long long idleCur, unsigned long long totalPrev, unsigned long long totalCur);

    void CPUUpdate(std::vector<CPUCore>& cores);

private:
    std::vector<CPUCore> coresLastCycle;
};
