#pragma once
#ifndef SHADOW_MAPPING_MANAGER_H
#define SHADOW_MAPPING_MANAGER_H

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LightManager.h"

class ShadowMappingManager
{
private:
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;
	Shader* _simpleDepthShader;
	Shader* _debugDepthQuad;

	void createShaders();
	void createDepthMap();
	void configureShaders();
public:
	unsigned int DepthMapFBO;
	unsigned int DepthMap;
	LightManager* Lights;

	ShadowMappingManager();
	~ShadowMappingManager();
	void RenderSceneToTextureBegin(glm::mat4 lightSpaceMatrix);
	void RenderSceneToTextureEnd();
	void RenderShadowMappDebug(float nearPlane, float farPlane);
};

#endif
