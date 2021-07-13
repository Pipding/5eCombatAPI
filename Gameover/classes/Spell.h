#pragma once
#include <string>
#include <vector>

#include "..\enums\e_SpellComponent.h"
#include "Character.h"

namespace SpellCasting {
    struct Spell
    {
        std::string name;
        int level;
        int castingTime;
        int range;
        std::vector<SpellComponent> components;
        int duration;
        bool requiresConcentration;

        //TODO: Refactor, spells shouldn't know about Characters. Other way around.
        void(*cast)(Spell*, Characters::Character*);

        inline bool operator==(Spell* a) {
            if (this == NULL && a != NULL)
                return false;

            if (a->name == this->name)
                return true;
            else
                return false;
        }

        inline bool operator!=(Spell* a) {
            if (this == NULL && a != NULL)
                return true;

            if (a->name != this->name)
                return true;
            else
                return false;
        }
    };
}