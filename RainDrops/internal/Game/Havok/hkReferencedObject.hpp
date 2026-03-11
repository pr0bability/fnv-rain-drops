#pragma once

#include "hkBaseObject.hpp"

class hkReferencedObject : public hkBaseObject {
public:
	virtual uint32_t	getClassType() const;
	virtual void		calcContentStatistics(void* collector, void* cls);

	uint16_t			m_memSizeAndFlags;
	mutable int16_t		m_referenceCount;
};

ASSERT_SIZE(hkReferencedObject, 0x8);