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

    float cpuUsage = 100*((LinuxParser::ActiveJiffies(pid)*1.0/CLOCKS_PER_SEC)/LinuxParser::UpTime(pid));
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
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }