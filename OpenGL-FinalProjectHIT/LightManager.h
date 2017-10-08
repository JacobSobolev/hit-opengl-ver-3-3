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
	ImVec4 ClearColor;
	DirectionalLight* LightDirectionalLight;
	vector<PointLight*>* LightPointLights;
	SpotLight* LightSpotLight;
	int LightPresets;

	unsigned int DepthMap;
	Shader* SimpleDepthShader;
	bool EnableShadows;
	bool EnableShadowMapDebug;

	float NearShadowPlane;
	float FarShadowPlane;
	float ShadowRegion;

	LightManager(Camera *camera);
	~LightManager();
	void InitLights();
	void UpdateSpotLight();
	void RenderLights();
	void SetThemePreset(int presetIndex);
	void RenderShadwoMapToTextureBegin();
	void RenderShadwoMapToTextureEnd();
	void RenderShadowMappDebug();
	glm::mat4 GetLightSpaceMatrix();
};

#endif