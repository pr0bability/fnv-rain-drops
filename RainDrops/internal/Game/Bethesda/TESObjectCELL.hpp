#pragma once

#include "bhkWorld.hpp"

class TESObjectCELL {
public:
	uint32_t pad[56];

	bhkWorld* GetbhkWorld() const;
};

ASSERT_SIZE(TESObjectCELL, 0xE0);