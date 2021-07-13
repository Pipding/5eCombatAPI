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
#include "chance/Dice.h"
#include "data/SpellSlotMaximum.h"
#include "data/Weapons.h"
#include "input/CmdInput.h"
#include "data/spells.h"

#include "classes/Character.cpp"
#include "spells/WitchBolt.cpp"
#include "spells/MagicMissile.cpp"
#include "spells/IceKnife.cpp"
#include "spells/RayOfFrost.cpp"

int main()
{
    Chance::initialize();

    bool areEnemiesWithin5Feet = false;
    bool attackWithMagic = false;
    Ability attackAbility;
    Weapons::Weapon selectedWeapon;
    SpellCasting::Spell selectedSpell;
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

    weapons.push_back(Weapons::dagger);
    weapons.push_back(Weapons::rapier);
    weapons.push_back(Weapons::light_crossbow);

    for (auto& wep : weapons)
    {
        weaponNames.push_back(wep.name);
    }


    //Spells
    std::vector<SpellCasting::Spell> spells;
    std::vector<std::string> spellNames;

    spells.push_back(SpellCasting::Spells::witch_bolt);
    spells.push_back(SpellCasting::Spells::magic_missile);
    spells.push_back(SpellCasting::Spells::ice_knife);
    spells.push_back(SpellCasting::Spells::ray_of_frost);


    for (auto& spell : spells)
    {
        spellNames.push_back(spell.name);
    }
    
    //Character
    character.weaponProficiencies.push_back(Weapons::dagger);
    character.weaponProficiencies.push_back(Weapons::light_crossbow);


    for(;;)
    {
        if (character.concentratedSpell != nullptr)
        {
            std::cout << "You're concentrating on " << character.concentratedSpell->name << ". Would you like to continue doing so?(Y/N)\n";
            bool continueWithConcentratedSpell = UserInput::userInputYesNo();

            if (continueWithConcentratedSpell)
            {
                character.castSpell(character.concentratedSpell);
            }
            else
            {
                character.breakConcentration();
            }
        }
        else
        {
            std::cout << "What kind of attack is this?\n";
            attackWithMagic = UserInput::userInputChoice({ "Mundane", "Magic" });

            if (!attackWithMagic)
            {
                std::cout << "Choose a weapon\n";
                selectedWeapon = weapons[UserInput::userInputChoice(weaponNames)];
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

                    attackAbility = finesseAbilities[UserInput::userInputChoice(finesseAbilityNames)];
                }

                if (selectedWeapon.isRanged)
                {
                    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
                    areEnemiesWithin5Feet = UserInput::userInputYesNo();
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

                if (UserInput::userInputYesNo())
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
                selectedSpell = spells[UserInput::userInputChoice(spellNames)];

                if (!character.castSpell(&selectedSpell))
                {
                    std::cout << "Failed to cast " << selectedSpell.name << "\n";
                }
            }
        }
    }

    return 0;
}
