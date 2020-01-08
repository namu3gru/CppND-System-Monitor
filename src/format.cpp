#include <string>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int HH = seconds / 60 / 60;
    int MM = (seconds / 60) % 60;
    int SS = seconds % 60;
    std::string sec = "0";
    if(SS < 10)
    {
        sec = sec + std::to_string(SS);
    }
    else
    {
        sec = std::to_string(SS);
    }
    
    string elapsedTime = std::to_string(HH) + ":" + std::to_string(MM) + ":" + sec;
    return elapsedTime; 
    }