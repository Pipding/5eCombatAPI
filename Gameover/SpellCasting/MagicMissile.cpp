//TODO: Pragma once probably wouldn't be necessary if you knew what you were doing
#pragma once

#include <iostream>

#include "Spell.h"
#include "../input/CmdInput.h"

namespace SpellCasting
{
    namespace CastFunctions
    {
        static void MagicMissile(SpellCasting::Spell* spell, Characters::Character* character)
        {
            //TODO: Mike Mearls says as written, magic missile uses one dice roll for damage
            auto damageRolls = character->rollIndividuatedSpellDamage((character->concentratedSpellCastLevel + 2), 4, 1);

            for (int i = 0; i < damageRolls.numberOfDice; ++i)
            {
                std::cout << "Dart " << i + 1 << ": " << damageRolls.rolls[i] << " force damage\n";
            }
        }
    }
}