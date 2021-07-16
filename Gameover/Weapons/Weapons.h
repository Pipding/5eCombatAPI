#pragma once

#include "Weapon.h"

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

    static Weapons::Weapon dart = {
        "Dart",
        true,
        Weapons::WeaponCategory::simple,
        "1d4",
        DamageType::piercing,
        {
            Weapons::WeaponProperty::finesse,
            Weapons::WeaponProperty::range,
            Weapons::WeaponProperty::thrown
        }
    };

    static Weapons::Weapon sling = {
        "Sling",
        true,
        Weapons::WeaponCategory::simple,
        "1d4",
        DamageType::bludgeoning,
        {
            Weapons::WeaponProperty::range
        }
    };

    static Weapons::Weapon quarterstaff = {
        "Quarterstaff",
        false,
        Weapons::WeaponCategory::simple,
        "1d6",
        DamageType::bludgeoning,
        {
            Weapons::WeaponProperty::versatile
        }
    };
}