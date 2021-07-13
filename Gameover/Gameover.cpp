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
#include "classes/Character.cpp"
#include "data/SpellSlotMaximum.h"
#include "data/Weapons.h"
#include "input/CmdInput.h"

void castWitchBolt(SpellCasting::Spell* spell, Characters::Character* character) 
{
    
    if (character->concentratedSpell == spell)
    {
        std::cout << Chance::rollDice("1d12") << " lightning damage\n";
    }
    else
    {
        std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
        int attackRoll = character->rangedSpellAttack(UserInput::userInputYesNo());

        std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
        if (UserInput::userInputYesNo())
        {
            int damageRoll = 0;

            for (int i = 0; i < character->concentratedSpellCastLevel; i++)
            {
                damageRoll += Chance::rollDice("1d12");
            }

            std::cout << damageRoll << " lightning damage\n";

            if (character->concentratedSpell != spell)
            {
                character->concentratedSpell = spell;
                std::cout << "Concentrating on " << spell->name << "\n";
            }
        }
    }
}

void castMagicMissile(SpellCasting::Spell* spell, Characters::Character* character) 
{
    //TODO: Mike Mearls says as written, magic missile uses one dice roll for damage
    auto damageRolls = character->rollIndividuatedSpellDamage((character->concentratedSpellCastLevel + 2), 4, 1);

    for (int i = 0; i < damageRolls.numberOfDice; ++i)
    {
        std::cout << "Dart " << i + 1 << ": " << damageRolls.rolls[i] << " force damage\n";
    }
}

void castIceKnife(SpellCasting::Spell* spell, Characters::Character* character)
{
    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
    int attackRoll = character->rangedSpellAttack(UserInput::userInputYesNo());

    std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
    if (UserInput::userInputYesNo())
    {
        std::cout << "Target takes " << Chance::rollDice("1d10") << " piercing damage\n";
        std::cout << "Target and all creatures within 5\' of target must beat a dexterity save of " << character->getSpellSaveDc() << " or take " << Chance::rollDice(std::to_string(character->concentratedSpellCastLevel + 1) + "d6") << " cold damage\n";
    }
}

void castRayOfFrost(SpellCasting::Spell* spell, Characters::Character* character)
{
    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
    int attackRoll = character->rangedSpellAttack(UserInput::userInputYesNo());

    std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
    if (UserInput::userInputYesNo())
    {
        std::cout << "Target takes " << Chance::rollDice("1d8") << " cold damage and speed is reduced by 10\' until next turn\n";
    }
}


int main()
{
    
    srand(time(NULL));

    //Randomness testing
    //std::map<int, int> diceResults;

    //for (int i = 0; i < 10000000; i++)
    //{
    //    diceResults[Chance::rollDice("1d20")] += 1;
    //}

    //for (auto const& pair : diceResults) {
    //    std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    //}


    bool running = true;
    bool continueWithConcentratedSpell;
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
    

    SpellCasting::Spell witch_bolt = {};
    witch_bolt.name = "Witch Bolt";
    witch_bolt.level = 1;
    witch_bolt.castingTime = 1;
    witch_bolt.range = 30;
    witch_bolt.components = { SpellComponent::verbal, SpellComponent::somatic, SpellComponent::material };
    witch_bolt.duration = 60;
    witch_bolt.requiresConcentration = true;
    witch_bolt.cast = castWitchBolt;

    spells.push_back(witch_bolt);

    SpellCasting::Spell magic_missile = {};
    magic_missile.name = "Magic Missile";
    magic_missile.level = 1;
    magic_missile.castingTime = 1;
    magic_missile.range = 120;
    magic_missile.components = { SpellComponent::verbal, SpellComponent::somatic };
    magic_missile.duration = 0;
    magic_missile.requiresConcentration = false;
    magic_missile.cast = castMagicMissile;

    spells.push_back(magic_missile);

    SpellCasting::Spell ice_knife = {};
    ice_knife.name = "Ice Knife";
    ice_knife.level = 1;
    ice_knife.castingTime = 1;
    ice_knife.range = 60;
    ice_knife.components = { SpellComponent::somatic, SpellComponent::material };
    ice_knife.duration = 0;
    ice_knife.requiresConcentration = false;
    ice_knife.cast = castIceKnife;

    spells.push_back(ice_knife);

    SpellCasting::Spell ray_of_frost = {};
    ray_of_frost.name = "Ray of Frost";
    ray_of_frost.level = 0;
    ray_of_frost.castingTime = 1;
    ray_of_frost.range = 60;
    ray_of_frost.components = { SpellComponent::verbal, SpellComponent::somatic };
    ray_of_frost.duration = 0;
    ray_of_frost.requiresConcentration = false;
    ray_of_frost.cast = castRayOfFrost;

    spells.push_back(ray_of_frost);


    for (auto& spell : spells)
    {
        spellNames.push_back(spell.name);
    }


    //Character
    //character.weaponProficiencies.push_back(dagger);
    character.weaponProficiencies.push_back(Weapons::dagger);
    character.weaponProficiencies.push_back(Weapons::light_crossbow);


    while (running)
    {
        //TODO: How to set concentrated spell in a way that sticks
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
                //This fails to resolve
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
