#pragma once

#include "bhkRefObject.hpp"
#include "Havok/hkSerializableCinfo.hpp"

class bhkWorld;
class hkpWorld;

class bhkSerializable : public bhkRefObject {
public:
	bhkSerializable();
	virtual ~bhkSerializable();

	virtual const hkpWorld*			GethkWorld() const;								// 37
	virtual hkpWorld*				GethkWorldRw() const;							// 38
	virtual bool					Add(bhkWorld* apWorld);							// 39
	virtual bool					Remove();										// 40
	virtual void					KillCinfo(bool abCreated);						// 41
	virtual uint32_t				GetCinfoSize();									// 42
	virtual uint32_t				LoadCinfo(NiStream* arStream);					// 43
	virtual void					Init(const hkSerializableCinfo& arInfo);		// 44
	virtual hkSerializableCinfo*	CreateCinfo(bool& arNotLoaded);					// 45
	virtual void					Kill();											// 46
	virtual void					PostInit(const hkSerializableCinfo& arInfo);	// 47
	virtual void					CopyCinfo(hkSerializableCinfo& arInfo);			// 48

	hkSerializableCinfo* pInfo;
};

ASSERT_SIZE(bhkSerializable, 0x10);