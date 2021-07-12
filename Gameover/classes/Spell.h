#pragma once
#include <string>
#include <vector>

#include "..\enums\e_SpellComponent.h"

struct Character;

struct Spell
{
    std::string name;
    int level;
    int castingTime;
    int range;
    std::vector<SpellComponent> components;
    int duration;
    bool requiresConcentration;

    void(*cast)(Spell, Character&);

    inline bool operator==(Spell a) {
        if (a.name == this->name)
            return true;
        else
            return false;
    }

    inline bool operator!=(Spell a) {
        if (a.name != this->name)
            return true;
        else
            return false;
    }
};