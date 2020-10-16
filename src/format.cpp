#include <string>

// #include <iostream>
// #include <iomanip>
// #include <ctime>
// #include <time.h>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    //seconds since epoch.
    time_t rawtime = seconds;

    //converts time_t to struct tm in UTC time
    struct tm * timeinfo = gmtime (&rawtime);

    //converts struct tm to string and 
    //takes substr with HH:MM:SS.
    return std::string(asctime(timeinfo)).substr(11,8); 
    
    //reference: http://cplusplus.com/reference/ctime/localtime/
}