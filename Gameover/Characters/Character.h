#pragma once

#include "../Weapons/Weapon.h"
#include "../SpellCasting/Spell.h"
#include "../SpellCasting/SpellSlotMaximum.h"
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
		Abilities::Ability spellcastingAbility; // TODO: Spellcasting ability could be a property of class
		std::map<Abilities::Ability, int> abilityScores;
		int proficiencyBonus;
		std::vector<Weapons::WeaponCategory> weaponCategoryProficiencies;
		std::vector<Weapons::Weapon> weaponProficiencies;
		SpellCasting::Spell* concentratedSpell;
		int concentratedSpellCastLevel;

		int level1SpellSlotMax = 4;
		int level2SpellSlotMax = 2;

		std::map<int, int> spellSlots = SpellCasting::MaximumSpellPoints::Sorcerer[3];

		int sorcererPoints = 3;

		int getAbilityModifier(Abilities::Ability ability);
		int getSpellSaveDc();
		bool isProficientWithWeapon(Weapons::Weapon weapon);
		bool castSpell(SpellCasting::Spell* spell);
		int rangedSpellAttack(bool enemiesWithin5Feet);
		void breakConcentration();
		bool handleConcentratedSpell();
		Chance::IndividuatedDiceRoll rollIndividuatedSpellDamage(int p_numberOfDice, int p_sizeOfDice, int p_plusNumber);
	};
}