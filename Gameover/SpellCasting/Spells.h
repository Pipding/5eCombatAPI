#pragma once

#include "Spell.h"
#include "WitchBolt.cpp"
#include "MagicMissile.cpp"
#include "IceKnife.cpp"
#include "RayOfFrost.cpp"

namespace SpellCasting
{
    namespace Spells {

        static SpellCasting::Spell witch_bolt =
        {
            "Witch Bolt",
            1,
            1,
            30,
            {
                SpellComponent::verbal,
                SpellComponent::somatic,
                SpellComponent::material
            },
            60,
            true,
            SpellCasting::CastFunctions::WitchBolt
        };


        static SpellCasting::Spell magic_missile =
        {
            "Magic Missile",
            1,
            1,
            120,
            {
                SpellComponent::verbal,
                SpellComponent::somatic
            },
            0,
            false,
            SpellCasting::CastFunctions::MagicMissile
        };


        static SpellCasting::Spell ice_knife = {
            "Ice Knife",
            1,
            1,
            60,
            { 
                SpellComponent::somatic, 
                SpellComponent::material 
            },
            0,
            false,
            SpellCasting::CastFunctions::IceKnife
        };


        static SpellCasting::Spell ray_of_frost = {
            "Ray of Frost",
            0,
            1,
            60,
            { 
                SpellComponent::verbal, 
                SpellComponent::somatic 
            },
            0,
            false,
            SpellCasting::CastFunctions::RayOfFrost
        };
    }
}