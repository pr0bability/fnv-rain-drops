#include "TESMain.hpp"

TESMain* TESMain::GetSingleton() {
	return *reinterpret_cast<TESMain**>(0x11DEA0C);
}

SceneGraph* TESMain::GetWorldSceneGraph() {
	return *reinterpret_cast<SceneGraph**>(0x11DEB7C);
}

NiCamera* TESMain::GetWorldRootCamera() {
	return GetWorldSceneGraph()->spCamera.m_pObject;
}


