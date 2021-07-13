#pragma once

#include <string>
#include <vector>
#include <map>

namespace Chance
{
    void initialize();
	int rollDice(std::string diceRollString);
	int rollDiceWithAdvantage(std::string diceRollString);
	int rollDiceWithDisadvantage(std::string diceRollString);

    struct IndividuatedDiceRoll
    {
        std::map<int, int> rolls;

        int numberOfDice;
        int sizeOfDice;
        int plusNumber;

        IndividuatedDiceRoll(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber);
        void reroll();
        void rerollSpecificDice(std::vector<int> diceIndices);

        std::string print();
    };
}