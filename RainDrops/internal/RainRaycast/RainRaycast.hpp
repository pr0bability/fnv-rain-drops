#pragma once

namespace RainRaycast {

	class RaycastThread {
	public:
		RaycastThread();
		~RaycastThread();

		bool bRunning	= true;
		HANDLE hThread	= nullptr;
		HANDLE hEvent	= nullptr;

		void Start();

	private:
		static DWORD WINAPI ThreadProc(LPVOID lpParameter);

		void Run();

		void CheckAboveCamera();
	};

	static constexpr uint32_t	FRAME_LIMIT = 15;
	static constexpr float		CHECK_DISTANCE = 8.0f * dM2NI;

	extern bool				bHit;
	extern RaycastThread*	pRaycastThread;
	
	void Initialize();
	void StartThread();

}
