#include "TES.hpp"

TES* TES::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<TES**>(0x11DEA10);
#else
	return *reinterpret_cast<TES**>(0xECF93C);
#endif
}

// GAME - 0x457070
// GECK - 0x4C98B0
TESObjectCELL* TES::GetCurrentCell() {
#ifdef GAME
    return ThisCall<TESObjectCELL*>(0x457070, this);
#else
    return ThisCall<TESObjectCELL*>(0x4C98B0, this);
#endif
}

// GAME - 0x4511E0
// GECK - 0x4C7C30
bool TES::IsCellLoaded(const TESObjectCELL* apCell, bool abIgnoreBuffered) const {
#ifdef GAME
    return ThisCall<bool>(0x4511E0, this, apCell, abIgnoreBuffered);
#else
    return ThisCall<bool>(0x4C7C30, this, apCell, abIgnoreBuffered);
#endif
}
