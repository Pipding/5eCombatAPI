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
        static void WitchBolt(SpellCasting::Spell* spell, Characters::Character* character)
        {
            if (character->concentratedSpell == spell)
            {
                std::cout << Chance::rollDice("1d12") << " lightning damage\n";
            }
            else
            {
                std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
                int attackRoll = character->rangedSpellAttack(UserInput::userInputYesNo());

                std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
                if (UserInput::userInputYesNo())
                {
                    int damageRoll = 0;

                    for (int i = 0; i < character->concentratedSpellCastLevel; i++)
                    {
                        damageRoll += Chance::rollDice("1d12");
                    }

                    std::cout << damageRoll << " lightning damage\n";

                    if (character->concentratedSpell != spell)
                    {
                        character->concentratedSpell = spell;
                        std::cout << "Concentrating on " << spell->name << "\n";
                    }
                }
            }
        }
    }
}