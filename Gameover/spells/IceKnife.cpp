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
        static void IceKnife(SpellCasting::Spell* spell, Characters::Character* character)
        {
            std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
            int attackRoll = character->rangedSpellAttack(UserInput::userInputYesNo());

            std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
            if (UserInput::userInputYesNo())
            {
                std::cout << "Target takes " << Chance::rollDice("1d10") << " piercing damage\n";
                std::cout << "Target and all creatures within 5\' of target must beat a dexterity save of " << character->getSpellSaveDc() << " or take " << Chance::rollDice(std::to_string(character->concentratedSpellCastLevel + 1) + "d6") << " cold damage\n";
            }
        }
    }
}