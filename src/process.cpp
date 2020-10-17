#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//initializer 
Process::Process(int p){
    pid = p;
}

// DONE: Return this process's ID
int Process::Pid() {
    return pid;
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    // reference: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    // cpu_usage = 100 * ((total_time / Hertz) / seconds)

    float cpuUsage = ((LinuxParser::ActiveJiffies(pid)*1.0/sysconf(_SC_CLK_TCK))/this->UpTime());
    cpuUtilization = cpuUsage;
    return cpuUsage;
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// DONE: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid);
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return this->cpuUtilization < a.cpuUtilization;
}