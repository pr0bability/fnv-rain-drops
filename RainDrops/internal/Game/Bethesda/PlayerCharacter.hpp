#pragma once

#include "CFilter.hpp"
#include "Gamebryo/NiAVObject.hpp"

class PlayerCharacter {
public:
	PlayerCharacter();
	virtual ~PlayerCharacter();

	virtual bool	InitiateDialogue(void*, void*, void*, bool, bool, bool, void*);
	virtual void*	GetPerkList(bool abTeammate);

	uint32_t pad[915];

	static PlayerCharacter* GetSingleton();

	bool Is3rdPerson();

	NiAVObject* Get3D(bool abFirstPerson) const;

	CFilter GetCollisionFilter() const;
};

ASSERT_SIZE(PlayerCharacter, 0xE50);