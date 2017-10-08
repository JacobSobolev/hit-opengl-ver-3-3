#pragma once

#ifndef  RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <vector>
#include "camera.h"
#include "shader.h"
#include "LightManager.h"
#include "object.h"

class RenderObject
{
protected:
	Camera *_camera;
	Shader* _objectShader;

	void createShader();

	void setDirectionalLight();
	void setPointLights();
	void setPointLight(int index);
	void setSpotLight();
	void render();
public:
	LightManager* Lights;

	RenderObject(Camera* camera);
	~RenderObject();

	std::vector<Object*>* ObjectsVec;
	
};

#endif // ! RENDER_OBJECT_H