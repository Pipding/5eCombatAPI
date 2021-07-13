#pragma once

#include <string>
#include <vector>

namespace UserInput 
{
    bool is_number(const std::string& s);
    bool userInputYesNo();
    int userInputChoice(std::vector<std::string> choices);
    int userInputIntInRange(int min, int max);
}