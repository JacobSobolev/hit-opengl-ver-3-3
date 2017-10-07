#pragma once

#ifndef  RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <vector>
#include "camera.h"
#include "shader.h"
#include "LightManager.h"
#include "object.h"
#include "ShadowMappingManager.h"

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

public:
	LightManager* Lights;
	ShadowMappingManager* Shadow;

	RenderObject(Camera* camera);
	~RenderObject();

	std::vector<Object*>* ObjectsVec;
	void Render(glm::mat4 projection, glm::mat4 view, glm::mat4 lightSpaceMatrix);
};

#endif // ! RENDER_OBJECT_H