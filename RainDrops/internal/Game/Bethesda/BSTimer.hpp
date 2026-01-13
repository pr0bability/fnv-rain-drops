#pragma once

class BSTimer {
public:
	uint8_t		ucDisableCounter;
	float		fClamp;
	float		fClampRemainder;
	float		fDelta;
	float		fDeltaUnmodified;
	uint32_t	uiLastTime;
	uint32_t	uiFirstTime;
	bool		bUseGlobalTimeMultiplierTarget;
	bool		bZeroDelta;
};

ASSERT_SIZE(BSTimer, 0x20);