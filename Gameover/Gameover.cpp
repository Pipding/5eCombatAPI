#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <functional>

#include "enums/e_Ability.h"
#include "enums/e_CharacterClass.h"
#include "enums/e_SpellComponent.h"
#include "enums/e_DamageType.h"
#include "enums/e_WeaponProperty.h"
#include "enums/e_WeaponCategory.h"
#include "classes/Spell.h"
#include "classes/Weapon.h"
#include "classes/Character.cpp"
#include "data/SpellSlotMaximum.h"
#include "chance/Dice.h"



bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool userInputYesNo()
{
    std::vector<std::string> acceptableResponses = { "y", "yes", "n", "no" };
    std::string userInputStr = "nil";

    std::transform(userInputStr.begin(), userInputStr.end(), userInputStr.begin(),
        [](unsigned char c) { return std::tolower(c); });

    while (std::find(acceptableResponses.begin(), acceptableResponses.end(), userInputStr) == acceptableResponses.end())
    {
        std::cin >> userInputStr;
    }

    return userInputStr == "y" || userInputStr == "yes";
}

int userInputChoice(std::vector<std::string> choices)
{
    std::string userInputStr;
    int userSelection = -1;
    for (int i = 0; i < choices.size(); i++)
    {
        std::cout << i << ": " << choices[i] << "\n";
    }

    while (userSelection < 0 || userSelection >= choices.size())
    {
        std::cin >> userInputStr;

        if (is_number(userInputStr))
        {
            userSelection = atoi(userInputStr.c_str());
        }
    }

    return userSelection;
}

int userInputIntInRange(int min, int max)
{
    std::string userInputStr;
    int userInputInt;

    do {
        do {
            std::cin >> userInputStr;
        } while (!is_number(userInputStr));

        userInputInt = atoi(userInputStr.c_str());

    } while (userInputInt < min || userInputInt > max);

    return userInputInt;

}


void castWitchBolt(Spell spell, Characters::Character& character) 
{
    if (character.concentratedSpell == spell)
    {
        std::cout << Chance::rollDice("1d12") << " lightning damage\n";
    }
    else
    {
        std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
        int attackRoll = character.rangedSpellAttack(userInputYesNo());

        std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
        if (userInputYesNo())
        {
            int damageRoll = 0;

            for (int i = 0; i < character.concentratedSpellCastLevel; i++)
            {
                damageRoll += Chance::rollDice("1d12");
            }

            std::cout << damageRoll << " lightning damage\n";

            if (character.concentratedSpell != spell)
            {
                character.concentratedSpell = spell;
                std::cout << "Concentrating on " << spell.name << "\n";
            }
        }
    }
}

void castMagicMissile(Spell spell, Characters::Character& character) 
{
    //TODO: Mike Mearls says as written, magic missile uses one dice roll for damage
    auto damageRolls = character.rollIndividuatedSpellDamage((character.concentratedSpellCastLevel + 2), 4, 1);

    for (int i = 0; i < damageRolls.numberOfDice; ++i)
    {
        std::cout << "Dart " << i + 1 << ": " << damageRolls.rolls[i] << " force damage\n";
    }
}

void castIceKnife(Spell spell, Characters::Character& character)
{
    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
    int attackRoll = character.rangedSpellAttack(userInputYesNo());

    std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
    if (userInputYesNo())
    {
        std::cout << "Target takes " << Chance::rollDice("1d10") << " piercing damage\n";
        std::cout << "Target and all creatures within 5\' of target must beat a dexterity save of " << character.getSpellSaveDc() << " or take " << Chance::rollDice(std::to_string(character.concentratedSpellCastLevel + 1) + "d6") << " cold damage\n";
    }
}

void castRayOfFrost(Spell spell, Characters::Character& character)
{
    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
    int attackRoll = character.rangedSpellAttack(userInputYesNo());

    std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
    if (userInputYesNo())
    {
        std::cout << "Target takes " << Chance::rollDice("1d8") << " cold damage and speed is reduced by 10\' until next turn\n";
    }
}


int main()
{
    
    srand(time(NULL));

    bool running = true;
    bool continueWithConcentratedSpell;
    bool areEnemiesWithin5Feet = false;
    bool attackWithMagic = false;
    Ability attackAbility;
    Weapons::Weapon selectedWeapon;
    Spell selectedSpell;
    int attackRollDiceResult;
    int attackAbilityModifier;
    int attackRoll;

    //Weapons
    std::vector<Weapons::Weapon> weapons;
    std::vector<std::string> weaponNames;

    Characters::Character character = {};
    character.spellcastingAbility = Ability::charisma;
    character.abilityScores[Ability::strength] = 15;
    character.abilityScores[Ability::dexterity] = 12;
    character.abilityScores[Ability::constitution] = 15;
    character.abilityScores[Ability::intelligence] = 12;
    character.abilityScores[Ability::wisdom] = 10;
    character.abilityScores[Ability::charisma] = 18;
    character.proficiencyBonus = 2;


    Weapons::Weapon dagger = {};
    dagger.name = "Dagger";
    dagger.isRanged = false;
    dagger.category = Weapons::WeaponCategory::simple;
    dagger.damage = "1d4";
    dagger.damageType = piercing;
    dagger.properties.push_back(Weapons::WeaponProperty::finesse);
    dagger.properties.push_back(Weapons::WeaponProperty::light);
    dagger.properties.push_back(Weapons::WeaponProperty::range);
    dagger.properties.push_back(Weapons::WeaponProperty::thrown);

    Weapons::Weapon rapier= {};
    rapier.name = "Rapier";
    rapier.isRanged = false;
    rapier.category = Weapons::WeaponCategory::martial;
    rapier.damage = "1d8";
    rapier.damageType = piercing;
    rapier.properties.push_back(Weapons::WeaponProperty::finesse);

    Weapons::Weapon light_crossbow = {};
    light_crossbow.name = "Light Crossbow";
    light_crossbow.isRanged = true;
    light_crossbow.category = Weapons::WeaponCategory::simple;
    light_crossbow.damage = "1d8";
    light_crossbow.damageType = piercing;
    light_crossbow.properties.push_back(Weapons::WeaponProperty::loading);
    light_crossbow.properties.push_back(Weapons::WeaponProperty::range);
    light_crossbow.properties.push_back(Weapons::WeaponProperty::two_handed);

    weapons.push_back(dagger);
    weapons.push_back(rapier);
    weapons.push_back(light_crossbow);

    for (auto& wep : weapons)
    {
        weaponNames.push_back(wep.name);
    }


    //Spells
    std::vector<Spell> spells;
    std::vector<std::string> spellNames;
    

    Spell witch_bolt = {};
    witch_bolt.name = "Witch Bolt";
    witch_bolt.level = 1;
    witch_bolt.castingTime = 1;
    witch_bolt.range = 30;
    witch_bolt.components = { verbal, somatic, material };
    witch_bolt.duration = 60;
    witch_bolt.requiresConcentration = true;
    witch_bolt.cast = castWitchBolt;

    spells.push_back(witch_bolt);

    Spell magic_missile = {};
    magic_missile.name = "Magic Missile";
    magic_missile.level = 1;
    magic_missile.castingTime = 1;
    magic_missile.range = 120;
    magic_missile.components = { verbal, somatic };
    magic_missile.duration = 0;
    magic_missile.requiresConcentration = false;
    magic_missile.cast = castMagicMissile;

    spells.push_back(magic_missile);

    Spell ice_knife = {};
    ice_knife.name = "Ice Knife";
    ice_knife.level = 1;
    ice_knife.castingTime = 1;
    ice_knife.range = 60;
    ice_knife.components = { somatic, material };
    ice_knife.duration = 0;
    ice_knife.requiresConcentration = false;
    ice_knife.cast = castIceKnife;

    spells.push_back(ice_knife);

    Spell ray_of_frost = {};
    ray_of_frost.name = "Ray of Frost";
    ray_of_frost.level = 0;
    ray_of_frost.castingTime = 1;
    ray_of_frost.range = 60;
    ray_of_frost.components = { verbal, somatic };
    ray_of_frost.duration = 0;
    ray_of_frost.requiresConcentration = false;
    ray_of_frost.cast = castRayOfFrost;

    spells.push_back(ray_of_frost);


    for (auto& spell : spells)
    {
        spellNames.push_back(spell.name);
    }


    //Character
    character.weaponProficiencies.push_back(dagger);
    character.weaponProficiencies.push_back(light_crossbow);


    while (running)
    {
        if (character.concentratedSpell == witch_bolt)
        {
            std::cout << "You're concentrating on " << character.concentratedSpell.name << ". Would you like to continue doing so?(Y/N)\n";
            bool continueWithConcentratedSpell = userInputYesNo();

            if (continueWithConcentratedSpell)
            {
                character.castSpell(witch_bolt);
            }
            else
            {
                character.breakConcentration();
            }
        }
        else
        {
            std::cout << "What kind of attack is this?\n";
            attackWithMagic = userInputChoice({ "Mundane", "Magic" });

            if (!attackWithMagic)
            {
                std::cout << "Choose a weapon\n";
                selectedWeapon = weapons[userInputChoice(weaponNames)];
                std::cout << "Attacking with " << selectedWeapon.name << " ...\n";

                //Set as defaults
                if (!selectedWeapon.isRanged)
                {
                    attackAbility = Ability::strength;
                }
                else
                {
                    attackAbility = Ability::dexterity;
                }

                //Does weapon have finesse attribute?
                if (std::find(selectedWeapon.properties.begin(), selectedWeapon.properties.end(), Weapons::WeaponProperty::finesse) != selectedWeapon.properties.end())
                {
                    Ability finesseAbilities[2] = { Ability::strength, Ability::dexterity };
                    std::vector<std::string> finesseAbilityNames = { "Strength", "Dexterity" };

                    int strMod = character.getAbilityModifier(Ability::strength);
                    int dexMod = character.getAbilityModifier(Ability::dexterity);

                    std::string strModText = (strMod <= 0) ? (std::to_string(strMod)) : ("+" + std::to_string(strMod));
                    std::string dexModText = (dexMod <= 0) ? (std::to_string(dexMod)) : ("+" + std::to_string(dexMod));

                    std::cout << selectedWeapon.name << " is a finesse weapon, would you like to use strength(" << strModText << ") or dexterity(" << dexModText << ")?\n";

                    attackAbility = finesseAbilities[userInputChoice(finesseAbilityNames)];
                }

                if (selectedWeapon.isRanged)
                {
                    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
                    areEnemiesWithin5Feet = userInputYesNo();
                }

                if (selectedWeapon.isRanged && areEnemiesWithin5Feet)
                {
                    std::cout << "Rolling to attack with disadvantage\n";
                    attackRollDiceResult = Chance::rollDiceWithDisadvantage("1d20");
                }
                else
                {
                    std::cout << "Rolling to attack\n";
                    attackRollDiceResult = Chance::rollDice("1d20");
                }

                std::cout << "Rolled " << attackRollDiceResult << "\n";

                attackAbilityModifier = character.getAbilityModifier(attackAbility);

                std::cout << "Adding " << Ability_str(attackAbility) << " mod of " << attackAbilityModifier << "\n";

                attackRoll = (attackRollDiceResult + attackAbilityModifier);

                if (character.isProficientWithWeapon(selectedWeapon))
                {
                    attackRoll += character.proficiencyBonus;
                    std::cout << "Adding proficiency bonus of " << character.proficiencyBonus << "\n";
                }

                std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";

                if (userInputYesNo())
                {
                    std::cout << "Rolling damage\n";
                    int damageRoll = Chance::rollDice(selectedWeapon.damage);
                    std::cout << "Rolled " << damageRoll << "\n";

                    std::cout << "Adding " << Ability_str(attackAbility) << " modifier\n";
                    damageRoll += attackAbilityModifier;

                    std::cout << damageRoll << " damage\n";
                }
            }
            else
            {
                std::cout << "Choose a spell\n";
                selectedSpell = spells[userInputChoice(spellNames)];

                if (!character.castSpell(selectedSpell))
                {
                    std::cout << "Failed to cast " << selectedSpell.name << "\n";
                }

            }
        }
    }

    return 0;
}
