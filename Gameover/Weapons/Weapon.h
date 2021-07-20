#pragma once
#include <string>
#include <vector>

#include "../enums/e_WeaponCategory.h"
#include "../enums/e_DamageType.h"
#include "../enums/e_WeaponProperty.h"

namespace Weapons
{
    struct Weapon
    {
        std::string name;
        bool isRanged;
        WeaponCategory category;
        std::string damage;
        DamageType damageType;
        std::vector<WeaponProperty> properties;

        inline bool operator==(Weapon a) {
            if (a.name == this->name && a.isRanged == this->isRanged && a.category == this->category && a.damage == this->damage && a.damageType == this->damageType)
                return true;
            else
                return false;
        }

        inline bool hasFinesseProperty() {
            return std::find(properties.begin(), properties.end(), Weapons::WeaponProperty::finesse) != properties.end();
        }
    };
}