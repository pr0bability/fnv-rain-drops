#include "ImageSpaceShader.hpp"

void ImageSpaceShader::LoadShadersEx(const char* apShaderName) {
	char cName[128];
	snprintf(cName, sizeof(cName), "RainDrops\\IS%s.vso", apShaderName);
	spVertexShader = pCreateVertexShader(cName);
	
	snprintf(cName, sizeof(cName), "RainDrops\\IS%s.pso", apShaderName);
	spPixelShader = pCreatePixelShader(cName);
}
