#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include "CPU.hpp"

int main()
{
    CPU cpuClass;
    //CPU::CPUCore cpu = cpuClass.parseCores()[0];
    
    //std::cout << cpu.id << ' ';
    //std::cout << cpu.user << ' ';
    //std::cout << cpu.nice << ' ';
    //std::cout << cpu.system << ' ';
    //std::cout << cpu.idle << ' ';
    //std::cout << cpu.iowait << ' ';
    //std::cout << cpu.irq << ' ';
    //std::cout << cpu.softirq << ' ';
    //std::cout << cpu.steal << ' ';
    //std::cout << cpu.guest << ' ';
    //std::cout << cpu.guest_nice << std::endl;
    
    bool quit = false;
    std::vector<CPU::CPUCore> cores;
    
    int debugCounter = 0;
    while (!quit) {
	cpuClass.CPUUpdate(cores);

	for (int i = 0; i < cores.size(); ++i) {
	    std::cout << cores[i].id << ": " << std::fixed << std::setprecision(2) << cores[i].usagePercent << " %" << std::endl;
	}

	debugCounter++;
	if (debugCounter >= 15) {
	    quit = true;
	}

	// TODO - detect user quit command
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
