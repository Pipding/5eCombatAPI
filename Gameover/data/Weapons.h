#pragma once

#include "../classes/Weapon.h"

namespace Weapons
{
    static Weapons::Weapon dagger = { 
        "Dagger", 
        false, 
        Weapons::WeaponCategory::simple, 
        "1d4", 
        DamageType::piercing, 
        {
            Weapons::WeaponProperty::finesse, 
            Weapons::WeaponProperty::light, 
            Weapons::WeaponProperty::range, 
            Weapons::WeaponProperty::thrown
        } 
    };


    static Weapons::Weapon light_crossbow = {
        "Light Crossbow",
        true,
        Weapons::WeaponCategory::simple,
        "1d8",
        DamageType::piercing,
        {
            Weapons::WeaponProperty::loading,
            Weapons::WeaponProperty::range,
            Weapons::WeaponProperty::two_handed
        }
    };


    static Weapons::Weapon rapier = {
        "Rapier",
        false,
        Weapons::WeaponCategory::martial,
        "1d8",
        DamageType::piercing,
        {
            Weapons::WeaponProperty::finesse
        }
    };
}