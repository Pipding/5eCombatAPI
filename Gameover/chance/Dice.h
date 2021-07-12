#pragma once
#include <string>


namespace Chance
{
	int rollDice(std::string diceRollString);
	int rollDiceWithAdvantage(std::string diceRollString);
	int rollDiceWithDisadvantage(std::string diceRollString);

	struct IndividuatedDiceRoll;
}