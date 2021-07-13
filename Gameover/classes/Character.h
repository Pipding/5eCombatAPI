#pragma once

//#include "Spell.h"
#include "Weapon.h"
#include "../data/SpellSlotMaximum.h"
#include "../enums/e_Ability.h"
#include "../chance/Dice.h"

namespace SpellCasting
{
	struct Spell;
}

namespace Characters
{
	struct Character {

		//CharacterClass characterClass;
		Ability spellcastingAbility;
		std::map<Ability, int> abilityScores;
		int proficiencyBonus;
		std::vector<Weapons::WeaponCategory> weaponCategoryProficiencies;
		std::vector<Weapons::Weapon> weaponProficiencies;
		SpellCasting::Spell* concentratedSpell;
		int concentratedSpellCastLevel;

		int level1SpellSlotMax = 4;
		int level2SpellSlotMax = 2;

		std::map<int, int> spellSlots = SpellCasting::MaximumSpellPoints::Sorcerer[3];

		int sorcererPoints = 3;

		int getAbilityModifier(Ability ability);
		int getSpellSaveDc();
		bool isProficientWithWeapon(Weapons::Weapon weapon);
		bool castSpell(SpellCasting::Spell* spell);
		int rangedSpellAttack(bool enemiesWithin5Feet);
		void breakConcentration();
		Chance::IndividuatedDiceRoll rollIndividuatedSpellDamage(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber);
	};
}