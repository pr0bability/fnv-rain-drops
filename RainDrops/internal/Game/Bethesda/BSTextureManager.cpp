#include "BSTextureManager.hpp"

// GAME - 0xB6D5E0
// GECK - 0x906910
BSRenderedTexture* BSTextureManager::CreateRenderedTexture(NiDX9Renderer* apRenderer, uint32_t auiWidth, uint32_t auiHeight, uint32_t auiFlags, D3DFORMAT aeFormat, D3DFORMAT aeEDRAMFormat, eTextureType aeType, D3DMULTISAMPLE_TYPE aeMSAAType, NiRenderedTexture* apAliasTex, uint32_t auiAliasOffset) {
#ifdef GAME
	return ThisCall<BSRenderedTexture*>(0xB6D5E0, this, apRenderer, auiWidth, auiHeight, auiFlags, aeFormat, aeEDRAMFormat, aeType, aeMSAAType, apAliasTex, auiAliasOffset);
#else
	return ThisCall<BSRenderedTexture*>(0x906910, this, apRenderer, auiWidth, auiHeight, auiFlags, aeFormat, aeEDRAMFormat, aeType, aeMSAAType, apAliasTex, auiAliasOffset);
#endif
}


// GAME - 0xB6E110
// GECK - 0x907440
BSRenderedTexture* BSTextureManager::BorrowRenderedTexture(NiDX9Renderer* apRenderer, eTextureType aeType, uint32_t auiFlags, uint32_t unk, uint32_t auiBackgroundColor) {
#if GAME
	return ThisCall<BSRenderedTexture*>(0xB6E110, this, apRenderer, aeType, auiFlags, unk, auiBackgroundColor);
#else
	return ThisCall<BSRenderedTexture*>(0x907440, this, apRenderer, aeType, auiFlags, unk, auiBackgroundColor);
#endif
}

// GAME - 0xB6DA10
// GECK - 0x906D40
void BSTextureManager::ReturnRenderedTexture(BSRenderedTexture* apRenderedTexture) {
#if GAME
	ThisCall(0xB6DA10, this, apRenderedTexture);
#else
	ThisCall(0x906D40, this, apRenderedTexture);
#endif
}
