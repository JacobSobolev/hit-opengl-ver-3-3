#pragma once
#ifndef LIGHTS_SCENE_H
#define LIGHTS_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "textureLoader.h"
#include "camera.h"

class LightsScene
{
private:
	unsigned int _screenWidth;
	unsigned int _screenHeight;
	Camera *_camera;
	Shader* _lampShader;
	unsigned int _VBO;
	unsigned int _lightVAO;
public:
	LightsScene(unsigned int screenWidth, unsigned int screenHeight, Camera *camera);
	~LightsScene();
	void RenderScene();
};

#endif