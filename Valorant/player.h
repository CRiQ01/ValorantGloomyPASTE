#pragma once
#define FINLINE __forceinline

#include <cstdint>
#include <string>
#include "Driver/Driver.h"
#include "Game/cheat.hpp"
#include "Game/globals.hpp"



/*

class Enemy
{
public:
	FINLINE std::string GetCurrentWeapon()
	{
		auto Inventory = Read<intptr_t>(actor_ptr + offsets::Inventory);
		intptr_t CurrentEquip = Read<intptr_t>(Inventory + offsets::CurrentWeapon);
		int weaponObj = Read<int>(CurrentEquip + offsets::ObjID);
		return GetWeaponName(weaponObj);
	}

	FINLINE std::string GetCharacter()
	{
		int charObj = Read<int>(actor_ptr + offsets::ObjID);
		auto characterName = GetCharacterName(charObj);
		return characterName;
	}

	uintptr_t actor_ptr;
	uintptr_t mesh_ptr;
};

inline bool operator==(const Enemy& a, const Enemy& b) // Enemy-Operator
{
	if (a.actor_ptr == b.actor_ptr)
	{
		if (a.mesh_ptr == b.mesh_ptr)
			return true;
	}
	return false;
}*/