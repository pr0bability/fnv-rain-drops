#include "Sky.hpp"

Sky* Sky::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<Sky**>(0x11DEA20);
#else
	return *reinterpret_cast<Sky**>(0xEDF1DC);
#endif
}

// GAME - 0x59E950
bool Sky::GetIsRaining() const {
	return ThisCall<bool>(0x59E950, this);
}

// GAME - 0x59EA80
bool Sky::GetIsSnowing() const {
	return ThisCall<bool>(0x59EA80, this);
}

bool Sky::IsUnderwater() {
#ifdef GAME
	return *reinterpret_cast<bool*>(0x11FF8C4);
#else
	return *reinterpret_cast<bool*>(0xF2431C);
#endif
}
