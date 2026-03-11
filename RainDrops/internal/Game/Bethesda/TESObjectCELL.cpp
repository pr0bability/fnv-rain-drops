#include "TESObjectCELL.hpp"

// GAME - 0x4543C0
// GECK - 0x43F400
bhkWorld* TESObjectCELL::GetbhkWorld() const {
#ifdef GAME
    return ThisCall<bhkWorld*>(0x4543C0, this);
#else
    return ThisCall<bhkWorld*>(0x43F400, this);
#endif
}
