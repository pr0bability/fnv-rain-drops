#include "RainRaycast.hpp"
#include "Bethesda/bhkPickData.hpp"
#include "Bethesda/bhkWorld.hpp"
#include "Bethesda/bhkThreadMemoryRouter.hpp"
#include "Bethesda/CFilter.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/TES.hpp"
#include "Bethesda/TESMain.hpp"
#include "Bethesda/TESObjectCELL.hpp"
#include "Gamebryo/NiRenderer.hpp"
#include "Havok/hkpClosestRayHitCollector.hpp"

namespace RainRaycast {

	bool			bHit = false;
	RaycastThread*	pRaycastThread = nullptr;

	RaycastThread::RaycastThread() {
		hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		hThread = CreateThread(nullptr, 0, ThreadProc, this, 0, nullptr);
		
		if (hThread)
			SetThreadDescription(hThread, L"[FNV] Rain Drops Raycast Thread");
	}

	RaycastThread::~RaycastThread() {
		bRunning = false;
		SetEvent(hEvent);
		if (hThread) {
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = nullptr;
		}
		if (hEvent) {
			CloseHandle(hEvent);
			hEvent = nullptr;
		}
	}

	void RaycastThread::Start() {
		SetEvent(hEvent);
	}

	DWORD __stdcall RaycastThread::ThreadProc(LPVOID lpParameter) {
		RaycastThread* pThis = static_cast<RaycastThread*>(lpParameter);

		pThis->Run();

		return 0;
	}

	void RaycastThread::Run() {
		while (bRunning) {
			WaitForSingleObject(hEvent, INFINITE);
			CheckAboveCamera();
			ResetEvent(hEvent);
		}
	}

	void RaycastThread::CheckAboveCamera() {
		TESObjectCELL* pCell = TES::GetSingleton()->GetCurrentCell();
		if (!pCell || !TES::GetSingleton()->IsCellLoaded(pCell, true)) {
			bHit = false;
			return;
		}

		NiPointer<bhkWorld> pWorld = pCell->GetbhkWorld();
		if (!pWorld || !pWorld->bAdded || !pWorld->bEnabled || bhkWorld::fBSDeltaTime <= 0.f) {
			bHit = false;
			return;
		}

		static thread_local bhkThreadMemoryRouter kMemRouter;
		bhkThreadMemoryRouter::Init(kMemRouter);

		PlayerCharacter* pPlayer = PlayerCharacter::GetSingleton();

		NiPoint3 kOrigin = TESMain::GetWorldRootCamera()->m_kWorld.m_Translate;

		NiPoint3 kUp(0.f, 0.f, 1.f);
		NiPoint3 kLength = kUp * CHECK_DISTANCE;

		CFilter kFilter = pPlayer->GetCollisionFilter();
		kFilter.SetLayer(COL_LAYER::LAYER_CAMERAPICK);

		bhkPickData kPick;
		kPick.SetFrom(kOrigin);
		kPick.SetLength(kLength);
		kPick.SetFilter(kFilter);

		bHit = pWorld->PickObject(kPick);

		bhkThreadMemoryRouter::Quit(kMemRouter);
	}

	void Initialize() {
		pRaycastThread = new RaycastThread();
	}

	void StartThread() {
		if (!pRaycastThread || TESMain::iFrameCounter % FRAME_LIMIT != 0)
			return;

		Sky* pSky = Sky::GetSingleton();

		if (!pSky || pSky->IsUnderwater())
			return;

		bool bIsRaining = pSky->GetIsRaining();
		bool bIsSnowing = pSky->GetIsSnowing();
		bool bIsWet = bIsRaining || bIsSnowing;

		if (!bIsWet)
			return;

		pRaycastThread->Start();
	}

}