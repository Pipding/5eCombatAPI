#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace UserInput 
{
    bool is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    bool userInputYesNo()
    {
        std::vector<std::string> acceptableResponses = { "y", "yes", "n", "no" };
        std::string userInputStr = "nil";

        std::transform(userInputStr.begin(), userInputStr.end(), userInputStr.begin(),
            [](unsigned char c) { return std::tolower(c); });

        while (std::find(acceptableResponses.begin(), acceptableResponses.end(), userInputStr) == acceptableResponses.end())
        {
            std::cin >> userInputStr;
        }

        return userInputStr == "y" || userInputStr == "yes";
    }

    int userInputChoice(std::vector<std::string> choices)
    {
        std::string userInputStr;
        int userSelection = -1;
        for (int i = 0; i < choices.size(); i++)
        {
            std::cout << i << ": " << choices[i] << "\n";
        }

        while (userSelection < 0 || userSelection >= choices.size())
        {
            std::cin >> userInputStr;

            if (is_number(userInputStr))
            {
                userSelection = atoi(userInputStr.c_str());
            }
        }

        return userSelection;
    }

    int userInputIntInRange(int min, int max)
    {
        std::string userInputStr;
        int userInputInt;

        do {
            do {
                std::cin >> userInputStr;
            } while (!is_number(userInputStr));

            userInputInt = atoi(userInputStr.c_str());

        } while (userInputInt < min || userInputInt > max);

        return userInputInt;

    }
}