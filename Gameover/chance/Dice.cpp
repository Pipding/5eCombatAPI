#include "Dice.h"
#include <map>
#include <vector>

namespace Chance
{
    static int rollDice(std::string diceRollString)
    {
        int plusNumber = 0;
        int diceType = 0;
        int diceQuantity = 0;
        int totalDiceRoll = 0;

        if (diceRollString.find('+') != std::string::npos)
        {
            plusNumber = atoi(diceRollString.substr(diceRollString.find("+") + 1).c_str());
            diceRollString = diceRollString.substr(0, diceRollString.find("+"));
        }

        diceType = atoi(diceRollString.substr(diceRollString.find("d") + 1).c_str());
        diceRollString = diceRollString.substr(0, diceRollString.find("d"));
        diceQuantity = atoi(diceRollString.c_str());

        for (int i = 0; i < diceQuantity; i++)
        {
            totalDiceRoll += (rand() % diceType + 1);
        }

        return totalDiceRoll + plusNumber;
    }

    static int rollDiceWithAdvantage(std::string diceRollString)
    {
        return std::max(rollDice(diceRollString), rollDice(diceRollString));
    }

	static int rollDiceWithDisadvantage(std::string diceRollString)
	{
		return std::min(rollDice(diceRollString), rollDice(diceRollString));
	}

    struct IndividuatedDiceRoll
    {
        std::map<int, int> rolls;

        int numberOfDice;
        int sizeOfDice;
        int plusNumber;

        IndividuatedDiceRoll(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber)
        {
            numberOfDice = p_numberOfDice;
            sizeOfDice = p_sizeOfDice;
            plusNumber = p_plusNumber;

            reroll();
        }

        void reroll()
        {
            rolls = {};

            for (int i = 0; i < numberOfDice; ++i)
            {
                rolls.insert({ i, rollDice("1d" + std::to_string(sizeOfDice) + "+" + std::to_string(plusNumber)) });
            }
        }

        void rerollSpecificDice(std::vector<int> diceIndices)
        {
            for (int i = 0; i < diceIndices.size(); ++i)
            {
                rolls[diceIndices[i]] = rollDice("1d" + std::to_string(sizeOfDice) + "+" + std::to_string(plusNumber));
            }
        }

        std::string print()
        {
            std::string prettyPrintString = "";

            for (int i = 0; i < numberOfDice; ++i)
            {
                prettyPrintString += ("Die " + std::to_string(i + 1) + ": " + std::to_string(rolls[i]) + "\n");
            }

            return prettyPrintString;
        }
    };
}