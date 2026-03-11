#include "PlayerCharacter.hpp"

PlayerCharacter* PlayerCharacter::GetSingleton() {
	return *reinterpret_cast<PlayerCharacter**>(0x011DEA3C);
}

bool PlayerCharacter::Is3rdPerson() {
	return ThisCall<bool>(0x4EAF60, this);
}

// GAME - 0x950BB0
NiAVObject* PlayerCharacter::Get3D(bool abFirstPerson) const {
	return ThisCall<NiAVObject*>(0x950BB0, this, abFirstPerson);
}

CFilter PlayerCharacter::GetCollisionFilter() const {
	return ThisCall<CFilter>(0x931ED0, this);
}

float PlayerCharacter::GetLooking() const {
	return ThisCall<float>(0x931D70, this);
}