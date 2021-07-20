#include "Character.h"
#include "../input/CmdInput.h"

#include "../vendor/nlohmann/json.hpp"

#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Characters
{
    int Character::getAbilityModifier(Abilities::Ability ability)
    {
        return (this->abilityScores[ability] - 10) / 2;
    }

    int Character::getSpellSaveDc()
    {
        return 8 + getAbilityModifier(spellcastingAbility) + proficiencyBonus;
    }

    bool Characters::Character::isProficientWithWeapon(Weapons::Weapon weapon)
    {
        bool characterProficientWithWeaponCategory = false;
        bool characterProficientWithWeapon = false;

        characterProficientWithWeaponCategory = std::find(weaponCategoryProficiencies.begin(), weaponCategoryProficiencies.end(), weapon.category) != weaponCategoryProficiencies.end();

        if (!characterProficientWithWeaponCategory)
        {
            characterProficientWithWeapon = std::find(weaponProficiencies.begin(), weaponProficiencies.end(), weapon) != weaponProficiencies.end();
        }

        return characterProficientWithWeapon || characterProficientWithWeaponCategory;
    }

    bool Characters::Character::castSpell(SpellCasting::Spell* spell)
    {
        if (!spell->requiresConcentration)
        {
            breakConcentration();
        }

        //Do you have enough spell slots of at least the spell's level
        bool haveSpellSlotsToCastSpell = false;
        int lowestViableSpellSlot = spell->level;

        for (int i = spell->level; i <= 9; ++i)
        {
            if (spellSlots[i] > 0) {
                haveSpellSlotsToCastSpell = true;
                lowestViableSpellSlot = i;
                break;
            }
        }

        if (!haveSpellSlotsToCastSpell) {
            return(false);
        }


        if (concentratedSpell != spell && spell->level != 0)
        {
            int userSelectedLevel = 0;
            //TODO: Set max here to be player's highest available spell level?
            std::cout << "What level are you casting at? (" << lowestViableSpellSlot << "-9" << ")\n";
            userSelectedLevel = UserInput::userInputIntInRange(lowestViableSpellSlot, 9);

            //Can the player cast the spell at the requested level?
            while (spellSlots[userSelectedLevel] == 0)
            {
                std::cout << "No level " << userSelectedLevel << " spell slots available\n";
                std::cout << "What level are you casting at? (" << lowestViableSpellSlot << "-9" << ")\n";
                userSelectedLevel = UserInput::userInputIntInRange(lowestViableSpellSlot, 9);
            }

            concentratedSpellCastLevel = userSelectedLevel;
            spellSlots[userSelectedLevel]--;

        }

        spell->cast(spell, this);

        return true;
    }

    int Characters::Character::rangedSpellAttack(bool enemiesWithin5Feet)
    {
        int spellAttackBonus = getAbilityModifier(spellcastingAbility) + proficiencyBonus;

        return spellAttackBonus + (enemiesWithin5Feet ? Chance::rollDiceWithDisadvantage("1d20") : Chance::rollDice("1d20"));
    }

    void Characters::Character::breakConcentration()
    {
        this->concentratedSpell = nullptr;
        this->concentratedSpellCastLevel = 0;
    }

    //Return true if a concentrated spell was handled
    bool Character::handleConcentratedSpell()
    {
        if (concentratedSpell != nullptr)
        {
            std::cout << "You're concentrating on " << concentratedSpell->name << ". Would you like to continue doing so?(Y/N)\n";
            bool continueWithConcentratedSpell = UserInput::userInputYesNo();

            if (continueWithConcentratedSpell)
            {
                castSpell(concentratedSpell);
            }
            else
            {
                breakConcentration();
            }
            return true;
        }
        return false;
    }

    //TODO: I think this should be moved to a CharacterClass class/struct
    Chance::IndividuatedDiceRoll Characters::Character::rollIndividuatedSpellDamage(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber)
    {
        auto damageRolls = Chance::IndividuatedDiceRoll(p_numberOfDice, p_sizeOfDice, p_plusNumber);

        if (sorcererPoints > 0)
        {
            int maxRerollableDice = std::min(getAbilityModifier(Abilities::Ability::charisma), p_numberOfDice);
            //TODO: Actually check if the character has Empowered Spell
            std::cout << damageRolls.print() << "\n";
            std::cout << "Use Empowered Spell to reroll up to " << maxRerollableDice << " damage dice? (Y/N)\n";
            bool playerWantsToRerollDamage = UserInput::userInputYesNo();

            if (playerWantsToRerollDamage)
            {
                sorcererPoints--;

                std::cout << "How many dice do you want to reroll? (1-" << maxRerollableDice << ")\n";
                unsigned int diceToReroll = UserInput::userInputIntInRange(1, maxRerollableDice);

                std::vector<int> diceToRerollIndices;

                if (diceToReroll != damageRolls.numberOfDice)
                {
                    while (diceToRerollIndices.size() < diceToReroll)
                    {
                        std::cout << "Which die do you want to reroll?\n";
                        int indexOfDiceToReroll = UserInput::userInputIntInRange(1, damageRolls.numberOfDice);

                        //Only do this if the vector doesn't alread contain this value
                        if (std::count(diceToRerollIndices.begin(), diceToRerollIndices.end(), indexOfDiceToReroll - 1) == 0)
                        {
                            diceToRerollIndices.push_back(indexOfDiceToReroll - 1);
                            std::cout << "Die " << std::to_string(indexOfDiceToReroll) << " marked for reroll\n";
                        }
                        else
                        {
                            std::cout << "Die " << std::to_string(indexOfDiceToReroll) << " is already marked for reroll\n";
                        }
                    }

                    damageRolls.rerollSpecificDice(diceToRerollIndices);
                }
                else
                {
                    damageRolls.reroll();
                }
            }
        }

        return damageRolls;
    }

    //int rollSummedSpellDamage()
    //{

    //}
}