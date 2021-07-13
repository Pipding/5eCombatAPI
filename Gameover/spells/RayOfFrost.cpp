//TODO: Pragma once probably wouldn't be necessary if you knew what you were doing
#pragma once

#include <iostream>

#include "../classes/Character.h"
#include "../classes/Spell.h"
#include "../input/CmdInput.h"

namespace SpellCasting
{
    namespace CastFunctions
    {
        static void RayOfFrost(SpellCasting::Spell* spell, Characters::Character* character)
        {
            std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
            int attackRoll = character->rangedSpellAttack(UserInput::userInputYesNo());

            std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
            if (UserInput::userInputYesNo())
            {
                std::cout << "Target takes " << Chance::rollDice("1d8") << " cold damage and speed is reduced by 10\' until next turn\n";
            }
        }
    }
}