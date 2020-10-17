#include "processor.h"
#include "linux_parser.h"

using LinuxParser::CPUStates;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    
    std::vector<long> cputil = LinuxParser::CpuUtilization();

    // CALCULATION: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    
    // Idle = idle + iowait
    long idle = cputil[CPUStates::kIdle_]+cputil[CPUStates::kIOwait_];
    
    // NonIdle = user + nice + system + irq + softirq + steal
    long nonIdle = cputil[CPUStates::kUser_]
                 + cputil[CPUStates::kNice_]
                 + cputil[CPUStates::kSystem_]
                 + cputil[CPUStates::kIRQ_]
                 + cputil[CPUStates::kSoftIRQ_]
                 + cputil[CPUStates::kSteal_];
   
    // Total = Idle + NonIdle
    long total = idle + nonIdle;

    // # differentiate: actual value minus the previous one
    // totald = Total - PrevTotal
    long totald = total - prevTotal;
    // idled = Idle - PrevIdle
    long idled = idle - prevIdle;
    
    // record for next cycle.
    prevIdle = idle;
    prevNonIdle = nonIdle;
    prevTotal = total;

    // CPU_Percentage = (totald - idled)/totald
    float cpuPerc = (totald - idled)*1.0/totald;
    
    return cpuPerc;
}