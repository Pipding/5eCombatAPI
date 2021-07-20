#pragma once

namespace Abilities {


	enum class Ability { strength, dexterity, constitution, intelligence, wisdom, charisma };
	enum class FinesseAttackAbilities { strength = (int)Ability::strength, dexterity = (int)Ability::dexterity };

	//static const std::map<std::string, Abilities::Ability> FinesseAttackAbilities = { {"Strength", Ability::strength}, {"Dexterity", Ability::dexterity} };

	static const char* Ability_str(Ability e)
	{
		//Lack of a default path in this switch is intentional. It will generate a warning if a case has been forgotten
		switch (e)
		{
		case Ability::strength:
			return "strength";
		case Ability::dexterity:
			return "dexterity";
		case Ability::constitution:
			return "constitution";
		case Ability::intelligence:
			return "intelligence";
		case Ability::wisdom:
			return "wisdom";
		case Ability::charisma:
			return "charisma";
		}
		return "invalid";
	}
}