#pragma once

#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>
#include "imgur/imgui.h"
#include "shader.h"
#include "textureLoader.h"
#include "camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "model.h"

class LightManager
{
private:
	Camera *_camera;
	Shader* _lampShader;
	Model* _lampModel;
	int _lastPreset;

	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;
	
	Shader* _debugDepthQuad;
	unsigned int _depthMapFBO;
	unsigned int _quadVAO;
	unsigned int _quadVBO;
	
	void createShaders();
	void createDepthMap();
	void configureShaders();
	void initClearColor();
	void initDirectionalLight();
	void createPointLights();
	void initPointLight(int index);
	void resetPointLights();
	void initSpotLight();
public:
	ImVec4 *ClearColor;
	DirectionalLight* LightDirectionalLight;
	vector<PointLight*>* LightPointLights;
	SpotLight* LightSpotLight;
	int LightPresets;

	

	unsigned int DepthMap;
	Shader* SimpleDepthShader;

	LightManager(Camera *camera);
	~LightManager();
	void InitLights();
	void UpdateSpotLight();
	void RenderLights(glm::mat4 projection, glm::mat4 view);
	void SetThemePreset(int presetIndex);
	void RenderShadwoMapToTextureBegin(glm::mat4 lightSpaceMatrix);
	void RenderShadwoMapToTextureEnd();
	void RenderShadowMappDebug(float nearPlane, float farPlane);
};

#endif