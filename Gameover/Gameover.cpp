#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <functional>

#include "vendor/nlohmann/json.hpp"

#include "enums/e_Ability.h"
#include "enums/e_CharacterClass.h"
#include "enums/e_SpellComponent.h"
#include "enums/e_DamageType.h"
#include "enums/e_WeaponProperty.h"
#include "enums/e_WeaponCategory.h"
#include "SpellCasting/Spell.h"
#include "Weapons/Weapon.h"
#include "chance/Dice.h"
#include "SpellCasting/SpellSlotMaximum.h"
#include "input/CmdInput.h"

#include "SpellCasting/spells.h"
#include "SpellCasting/spells.cpp"

#include "Weapons/Weapons.h"
#include "Weapons/Weapons.cpp"

#include "Characters/Character.cpp"
#include "SpellCasting/WitchBolt.cpp"
#include "SpellCasting/MagicMissile.cpp"
#include "SpellCasting/IceKnife.cpp"
#include "SpellCasting/RayOfFrost.cpp"

int main()
{
    bool attackWithMagic = false;
    Abilities::Ability attackAbility;
    Weapons::Weapon selectedWeapon;
    SpellCasting::Spell selectedSpell;
    int attackRollDiceResult;
    int attackAbilityModifier;
    int attackRoll;

    //Weapons
    std::vector<std::string> weaponNames;

    Characters::Character character = {};
    character.spellcastingAbility = Abilities::Ability::charisma;
    character.abilityScores[Abilities::Ability::strength] = 15;
    character.abilityScores[Abilities::Ability::dexterity] = 12;
    character.abilityScores[Abilities::Ability::constitution] = 15;
    character.abilityScores[Abilities::Ability::intelligence] = 12;
    character.abilityScores[Abilities::Ability::wisdom] = 10;
    character.abilityScores[Abilities::Ability::charisma] = 18;
    character.proficiencyBonus = 2;

    for (auto& wep : Weapons::AllWeapons)
    {
        weaponNames.push_back(wep.name);
    }

    //Spells
    std::vector<std::string> spellNames;

    for (auto& spell : SpellCasting::Spells::AllSpells)
    {
        spellNames.push_back(spell.name);
    }
    
    //Character
    character.weaponProficiencies.push_back(Weapons::dagger);
    character.weaponProficiencies.push_back(Weapons::light_crossbow);
    character.weaponProficiencies.push_back(Weapons::dart);
    character.weaponProficiencies.push_back(Weapons::quarterstaff);
    character.weaponProficiencies.push_back(Weapons::sling);


    for(;;)
    {
        if(!character.handleConcentratedSpell())
        {
            std::cout << "What kind of attack is this?\n";
            attackWithMagic = UserInput::userInputChoice({ "Mundane", "Magic" });

            if (!attackWithMagic)
            {
                std::cout << "Choose a weapon\n";
                selectedWeapon = Weapons::AllWeapons[UserInput::userInputChoice(weaponNames)];
                std::cout << "Attacking with " << selectedWeapon.name << " ...\n";

                //Set as defaults
                if (selectedWeapon.isRanged)
                    attackAbility = Abilities::Ability::dexterity;
                else
                    attackAbility = Abilities::Ability::strength;

                

                //Does weapon have finesse attribute?
                if (selectedWeapon.hasFinesseProperty())
                {
                    int strMod = character.getAbilityModifier(Abilities::Ability::strength);
                    int dexMod = character.getAbilityModifier(Abilities::Ability::dexterity);

                    std::string strModText = (strMod <= 0) ? (std::to_string(strMod)) : ("+" + std::to_string(strMod));
                    std::string dexModText = (dexMod <= 0) ? (std::to_string(dexMod)) : ("+" + std::to_string(dexMod));

                    std::cout << selectedWeapon.name << " is a finesse weapon, would you like to use strength(" << strModText << ") or dexterity(" << dexModText << ")?\n";

                    attackAbility = Abilities::Ability(UserInput::userInputChoice({"Strength", "Dexterity"})); //This only works because str = 0 and dex = 1
                }

                // This happens once here because it's more concise. 
                // It feels weird since rolling disadvantage for a ranged attack technically rolls 3 times, but the first would be discarded so it's ok?
                attackRollDiceResult = Chance::rollDice("1d20");

                if (selectedWeapon.isRanged)
                {
                    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
                    
                    if(UserInput::userInputYesNo())
                    {
                        std::cout << "Rolling to attack with disadvantage\n";
                        attackRollDiceResult = Chance::rollDiceWithDisadvantage("1d20");
                    }
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
                selectedSpell = SpellCasting::Spells::AllSpells[UserInput::userInputChoice(spellNames)];

                if (!character.castSpell(&selectedSpell))
                {
                    std::cout << "Failed to cast " << selectedSpell.name << "\n";
                }
            }
        }
    }

    return 0;
}
