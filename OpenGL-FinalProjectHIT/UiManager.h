#pragma once

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "imgur/imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <vector>
#include <iostream>
#include <string> 
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CubeManager.h"
#include "LightManager.h"
#include "FloorManager.h"
#include "camera.h"

class UiManager
{
private:

public:
	UiManager(GLFWwindow* window);
	~UiManager();

	Camera* Camera;
	LightManager* Lights;
	CubeManager* Cubes;
	FloorManager* Floor;
	
	
	void RenderBegin();
	void RenderSet();
	void RenderEnd();
	void CleanUp();
};

#endif