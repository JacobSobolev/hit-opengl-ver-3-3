#pragma once
#ifndef FLOOR_MANAGER_H
#define FLOOR_MANAGER_H


#include "camera.h"
#include "shader.h"
#include "textureLoader.h"
#include "LightManager.h"
#include "RenderObject.h"



class FloorManager : public RenderObject
{
private:

	unsigned int _diffuseMap;
	unsigned int _specularMap;
	unsigned int _VBO;
	unsigned int _VAO;

	void createObject();
	void createTexture();
	void initFloor();

public:
	FloorManager(Camera* camera);
	~FloorManager();

	
	void RenderFloor(glm::mat4 projection, glm::mat4 view, glm::mat4 lightSpaceMatrix);
	void RenderFloorForShadowMap();
};

#endif