#pragma once

#ifndef CUBE_MANAGER_H
#define CUBE_MANAGER_H

#include <iostream>
#include <sstream>
#include <string>


#include <glm/glm.hpp>

#include "shader.h"
#include "textureLoader.h"
#include "camera.h"
#include "LightManager.h"
#include "RenderObject.h"

class CubeManager : public RenderObject
{
private:
	unsigned int _diffuseMap1;
	unsigned int _specularMap1;
	unsigned int _diffuseMap2;
	unsigned int _specularMap2;
	unsigned int _diffuseMap3;
	unsigned int _specularMap3;

	unsigned int _VBO;
	unsigned int _VAO;

	int _lastTextureIndex;

	void createObject();
	void createTexture();
	void initCubes();
	
public:
	bool AnimateCubes;
	bool VisiableCubes;
	
	int TextureItem;

	CubeManager(Camera* camera);
	~CubeManager();
	void RenderCubes(float dt,glm::mat4 projection, glm::mat4 view, glm::mat4 lightSpaceMatrix);
	void RenderCubesForShadowMap(float dt);
	void SetTextureToAll(int textureIndex);
	void SetTextureToCube(int textureIndex,int cubeIndex);
	
	
};

#endif