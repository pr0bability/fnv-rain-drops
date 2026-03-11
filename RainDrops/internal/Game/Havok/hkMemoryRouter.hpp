#pragma once

class hkMemoryRouter {
public:
	hkMemoryRouter() {};
	~hkMemoryRouter() {};

	uint32_t pad[9];
};

ASSERT_SIZE(hkMemoryRouter, 0x24);
