#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <functional>

enum CharacterClass { barbarian, bard, cleric, druid, fighter, monk, paladin, ranger, rogue, sorcerer, warlock, wizard };
enum WeaponType { melee, ranged };
enum WeaponCategory { simple, martial };
enum WeaponProperty { ammunition, finesse, heavy, light, loading, range, reach, special, thrown, two_handed, versatile };
enum DamageType { acid, bludgeoning, cold, fire, force, lightning, necrotic, piercing, poison, psychic, radiant, slashing, thunder };
enum SpellComponent { verbal, somatic, material };
enum Ability { strength, dexterity, constitution, intelligence, wisdom, charisma };

std::map<Ability, std::string> AbilityName
{
    { strength, "Strength" }, 
    { dexterity, "Dexterity" },
    { constitution, "Constitution" },
    { intelligence, "Intelligence" },
    { wisdom, "Wisdom" },
    { charisma, "Charisma" }
};

struct Character;


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

int rollDice(std::string diceRollString)
{
    int plusNumber = 0;
    int diceType = 0;
    int diceQuantity = 0;
    int totalDiceRoll = 0;

    if (diceRollString.find('+') != std::string::npos)
    {
        plusNumber = atoi(diceRollString.substr(diceRollString.find("+") + 1).c_str());
        diceRollString = diceRollString.substr(0, diceRollString.find("+"));
    }

    diceType = atoi(diceRollString.substr(diceRollString.find("d") + 1).c_str());
    diceRollString = diceRollString.substr(0, diceRollString.find("d"));
    diceQuantity= atoi(diceRollString.c_str());

    for (int i = 0; i < diceQuantity; i++)
    {
        totalDiceRoll += (rand() % diceType + 1);
    }

    return totalDiceRoll + plusNumber;
}

int rollDiceWithAdvantage(std::string diceRollString)
{
    return std::max(rollDice(diceRollString), rollDice(diceRollString));
}

int rollDiceWithDisadvantage(std::string diceRollString)
{
    return std::min(rollDice(diceRollString), rollDice(diceRollString));
}

//Maybe have a different thing called a SummedDiceRoll
struct IndividuatedDiceRoll
{
    std::map<int, int> rolls;

    int numberOfDice;
    int sizeOfDice;
    int plusNumber;

    IndividuatedDiceRoll(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber)
    {
        numberOfDice = p_numberOfDice;
        sizeOfDice = p_sizeOfDice;
        plusNumber = p_plusNumber;

        reroll();
    }

    void reroll()
    {
        rolls = {};

        for (int i = 0; i < numberOfDice; ++i)
        {
            rolls.insert({ i, rollDice("1d" + std::to_string(sizeOfDice) + "+" + std::to_string(plusNumber)) });
        }
    }

    void rerollSpecificDice(std::vector<int> diceIndices)
    {
        for (int i = 0; i < diceIndices.size(); ++i)
        {
            rolls[diceIndices[i]] = rollDice("1d" + std::to_string(sizeOfDice) + "+" + std::to_string(plusNumber));
        }
    }

    std::string print()
    {
        std::string prettyPrintString = "";

        for (int i = 0; i < numberOfDice; ++i)
        {
            prettyPrintString += ("Die " + std::to_string(i+1) + ": " + std::to_string(rolls[i]) + "\n" );
        }

        return prettyPrintString;
    }
};

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

struct Weapon
{
    std::string name;
    WeaponType type;
    WeaponCategory category;
    std::string damage;
    DamageType damageType;
    std::vector<WeaponProperty> properties;

    inline bool operator==(Weapon a) {
        if (a.name == this->name && a.type == this->type && a.category == this->category && a.damage == this->damage && a.damageType == this->damageType)
            return true;
        else
            return false;
    }
};

struct Character
{
    //CharacterClass characterClass;
    Ability spellcastingAbility;

    std::map<Ability, int> abilityScores;
    int proficiencyBonus;
    std::vector<WeaponCategory> weaponCategoryProficiencies;
    std::vector<Weapon> weaponProficiencies;
    Spell concentratedSpell;
    int concentratedSpellCastLevel;

    int level1SpellSlotMax = 4;
    int level2SpellSlotMax = 2;

    std::map<int, int> spellSlots = {
        {1, 4},
        {2, 2},
        {3, 0},
        {4, 0},
        {5, 0},
        {6, 0},
        {7, 0},
        {8, 0},
        {9, 0}
    };

    int sorcererPoints = 3;



    int getAbilityModifier(Ability ability)
    {
        return (this->abilityScores[ability] - 10) / 2;
    }

    int getSpellSaveDc()
    {
        return 8 + getAbilityModifier(spellcastingAbility) + proficiencyBonus;
    }

    bool isProficientWithWeapon(Weapon weapon)
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

    bool castSpell(Spell spell)
    {
        if (!spell.requiresConcentration)
        {
            breakConcentration();
        }

        //Do you have enough spell slots of at least the spell's level
        bool haveSpellSlotsToCastSpell = false;
        int lowestViableSpellSlot = spell.level;

        for (int i = spell.level; i <= 9; ++i)
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


        if (concentratedSpell != spell && spell.level != 0)
        {
            int userSelectedLevel = 0;
            //TODO: Set max here to be player's highest available spell level?
            std::cout << "What level are you casting at? (" << lowestViableSpellSlot << "-9" << ")\n";
            userSelectedLevel = userInputIntInRange(lowestViableSpellSlot, 9);

            //Can the player cast the spell at the requested level?
            while (spellSlots[userSelectedLevel] == 0)
            {
                std::cout << "No level " << userSelectedLevel << " spell slots available\n";
                std::cout << "What level are you casting at? (" << lowestViableSpellSlot << "-9" << ")\n";
                userSelectedLevel = userInputIntInRange(lowestViableSpellSlot, 9);
            }

            concentratedSpellCastLevel = userSelectedLevel;
            spellSlots[userSelectedLevel]--;

        }

        spell.cast(spell, *this);

        return true;
    }

    int rangedSpellAttack(bool enemiesWithin5Feet)
    {        
        int spellAttackBonus = getAbilityModifier(spellcastingAbility) + proficiencyBonus;

        return spellAttackBonus + (enemiesWithin5Feet ? rollDiceWithDisadvantage("1d20") : rollDice("1d20"));
    }

    void breakConcentration()
    {
        this->concentratedSpell = {};
        this->concentratedSpellCastLevel = 0;
    }

    //TODO: I think this should be moved to a CharacterClass class/struct
    IndividuatedDiceRoll rollIndividuatedSpellDamage(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber)
    {
        IndividuatedDiceRoll damageRolls = IndividuatedDiceRoll(p_numberOfDice, p_sizeOfDice, p_plusNumber);

        if (sorcererPoints > 0)
        {
            int maxRerollableDice = std::min(getAbilityModifier(charisma), p_numberOfDice);
            //TODO: Actually check if the character has Empowered Spell
            std::cout << damageRolls.print() << "\n";
            std::cout << "Use Empowered Spell to reroll up to " << maxRerollableDice << " damage dice? (Y/N)\n";
            bool playerWantsToRerollDamage = userInputYesNo();

            if (playerWantsToRerollDamage)
            {
                sorcererPoints--;

                std::cout << "How many dice do you want to reroll? (1-" << maxRerollableDice << ")\n";
                int diceToReroll = userInputIntInRange(1, maxRerollableDice);

                std::vector<int> diceToRerollIndices;

                if (diceToReroll != damageRolls.numberOfDice)
                {
                    while (diceToRerollIndices.size() < diceToReroll)
                    {
                        std::cout << "Which die do you want to reroll?\n";
                        int indexOfDiceToReroll = userInputIntInRange(1, damageRolls.numberOfDice);

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

    int rollSummedSpellDamage()
    {

    }
};

void castWitchBolt(Spell spell, Character& character) 
{
    if (character.concentratedSpell == spell)
    {
        std::cout << rollDice("1d12") << " lightning damage\n";
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
                damageRoll += rollDice("1d12");
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

void castMagicMissile(Spell spell, Character& character) 
{
    //TODO: Mike Mearls says as written, magic missile uses one dice roll for damage
    IndividuatedDiceRoll damageRolls = character.rollIndividuatedSpellDamage((character.concentratedSpellCastLevel + 2), 4, 1);

    for (int i = 0; i < damageRolls.numberOfDice; ++i)
    {
        std::cout << "Dart " << i + 1 << ": " << damageRolls.rolls[i] << " force damage\n";
    }
}

void castIceKnife(Spell spell, Character& character)
{
    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
    int attackRoll = character.rangedSpellAttack(userInputYesNo());

    std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
    if (userInputYesNo())
    {
        std::cout << "Target takes " << rollDice("1d10") << " piercing damage\n";
        std::cout << "Target and all creatures within 5\' of target must beat a dexterity save of " << character.getSpellSaveDc() << " or take " << rollDice(std::to_string(character.concentratedSpellCastLevel + 1) + "d6") << " cold damage\n";
    }
}

void castRayOfFrost(Spell spell, Character& character)
{
    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
    int attackRoll = character.rangedSpellAttack(userInputYesNo());

    std::cout << "Rolled " << attackRoll << " to attack\n" << "Hit? (Y/N)" << "\n";
    if (userInputYesNo())
    {
        std::cout << "Target takes " << rollDice("1d8") << " cold damage and speed is reduced by 10\' until next turn\n";
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
    Weapon selectedWeapon;
    Spell selectedSpell;
    int attackRollDiceResult;
    int attackAbilityModifier;
    int attackRoll;

    //Weapons
    std::vector<Weapon> weapons;
    std::vector<std::string> weaponNames;

    Character character = {};
    character.spellcastingAbility = charisma;
    character.abilityScores[strength] = 15;
    character.abilityScores[dexterity] = 12;
    character.abilityScores[constitution] = 15;
    character.abilityScores[intelligence] = 12;
    character.abilityScores[wisdom] = 10;
    character.abilityScores[charisma] = 18;
    character.proficiencyBonus = 2;


    Weapon dagger = {};
    dagger.name = "Dagger";
    dagger.type = melee;
    dagger.category = simple;
    dagger.damage = "1d4";
    dagger.damageType = piercing;
    dagger.properties.push_back(finesse);
    dagger.properties.push_back(light);
    dagger.properties.push_back(range);
    dagger.properties.push_back(thrown);

    Weapon rapier= {};
    rapier.name = "Rapier";
    rapier.type = melee;
    rapier.category = martial;
    rapier.damage = "1d8";
    rapier.damageType = piercing;
    rapier.properties.push_back(finesse);

    Weapon light_crossbow = {};
    light_crossbow.name = "Light Crossbow";
    light_crossbow.type = ranged;
    light_crossbow.category = simple;
    light_crossbow.damage = "1d8";
    light_crossbow.damageType = piercing;
    light_crossbow.properties.push_back(loading);
    light_crossbow.properties.push_back(range);
    light_crossbow.properties.push_back(two_handed);

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
                if (selectedWeapon.type == melee)
                {
                    attackAbility = strength;
                }
                else
                {
                    attackAbility = dexterity;
                }

                //Does weapon have finesse attribute?
                if (std::find(selectedWeapon.properties.begin(), selectedWeapon.properties.end(), finesse) != selectedWeapon.properties.end())
                {
                    Ability finesseAbilities[2] = { strength, dexterity };
                    std::vector<std::string> finesseAbilityNames = { "Strength", "Dexterity" };

                    int strMod = character.getAbilityModifier(strength);
                    int dexMod = character.getAbilityModifier(dexterity);

                    std::string strModText = (strMod <= 0) ? (std::to_string(strMod)) : ("+" + std::to_string(strMod));
                    std::string dexModText = (dexMod <= 0) ? (std::to_string(dexMod)) : ("+" + std::to_string(dexMod));

                    std::cout << selectedWeapon.name << " is a finesse weapon, would you like to use strength(" << strModText << ") or dexterity(" << dexModText << ")?\n";

                    attackAbility = finesseAbilities[userInputChoice(finesseAbilityNames)];
                }

                if (selectedWeapon.type == ranged)
                {
                    std::cout << "Are there any enemies within 5' of you? (Y/N)\n";
                    areEnemiesWithin5Feet = userInputYesNo();
                }

                if (selectedWeapon.type == ranged && areEnemiesWithin5Feet)
                {
                    std::cout << "Rolling to attack with disadvantage\n";
                    attackRollDiceResult = rollDiceWithDisadvantage("1d20");
                }
                else
                {
                    std::cout << "Rolling to attack\n";
                    attackRollDiceResult = rollDice("1d20");
                }

                std::cout << "Rolled " << attackRollDiceResult << "\n";

                attackAbilityModifier = character.getAbilityModifier(attackAbility);

                std::cout << "Adding " << AbilityName[attackAbility] << " mod of " << attackAbilityModifier << "\n";

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
                    int damageRoll = rollDice(selectedWeapon.damage);
                    std::cout << "Rolled " << damageRoll << "\n";

                    std::cout << "Adding " << AbilityName[attackAbility] << " modifier\n";
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
