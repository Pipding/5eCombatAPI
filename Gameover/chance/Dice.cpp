#include "Dice.h"
#include <time.h>

namespace Chance
{
    static unsigned long x = 123456789, y = 362436069, z = 521288629;

    unsigned long xorshf96(void) {          //period 2^96-1
        unsigned long t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;

        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;

        return z;
    }

    void initialize() {
        srand(time(NULL));
        x = rand();
    }

    //TODO: Something seems fishy about this. d4s seem to roll 4 too often
    int rollDice(std::string diceRollString)
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
            totalDiceRoll += (xorshf96() % diceType + 1);
        }

        return totalDiceRoll + plusNumber;
    }

    static int rollDiceWithAdvantage(std::string diceRollString)
    {
        return std::max(rollDice(diceRollString), rollDice(diceRollString));
    }

	int rollDiceWithDisadvantage(std::string diceRollString)
	{
		return std::min(rollDice(diceRollString), rollDice(diceRollString));
	}

    IndividuatedDiceRoll::IndividuatedDiceRoll(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber)
    {
        numberOfDice = p_numberOfDice;
        sizeOfDice = p_sizeOfDice;
        plusNumber = p_plusNumber;

        rolls = {};

        reroll();
    }

    void IndividuatedDiceRoll::reroll()
    {
        rolls = {};

        for (int i = 0; i < numberOfDice; ++i)
        {
            rolls.insert({ i, rollDice("1d" + std::to_string(sizeOfDice) + "+" + std::to_string(plusNumber)) });
        }
    }

    void IndividuatedDiceRoll::rerollSpecificDice(std::vector<int> diceIndices)
    {
        for (int i = 0; i < diceIndices.size(); ++i)
        {
            rolls[diceIndices[i]] = rollDice("1d" + std::to_string(sizeOfDice) + "+" + std::to_string(plusNumber));
        }
    }

    std::string IndividuatedDiceRoll::print()
    {
        std::string prettyPrintString = "";

        for (int i = 0; i < numberOfDice; ++i)
        {
            prettyPrintString += ("Die " + std::to_string(i + 1) + ": " + std::to_string(rolls[i]) + "\n");
        }

        return prettyPrintString;
    }
}