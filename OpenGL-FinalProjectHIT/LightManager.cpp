#include "LightManager.h"

LightManager::LightManager(Camera *camera)
{
	ShadowRegion = 16.0f;
	NearShadowPlane = 1.0f;
	FarShadowPlane = 25.0f;

	LightDirectionalLight = new DirectionalLight;
	LightPointLights = new vector<PointLight*>();
	LightSpotLight = new SpotLight;
	_lastPreset = 0;
	LightPresets = 0;
	_camera = camera;
	_lampModel = new Model("bulb/bulb.obj");
	EnableShadowMapDebug = false;
	EnableShadows = true;
	createShaders();
	createDepthMap();
	configureShaders();
}


LightManager::~LightManager()
{
	glDeleteVertexArrays(1, &_quadVBO);
	glDeleteBuffers(1, &_quadVAO);
	delete(_lampModel);
	delete(_lampShader);
}

void LightManager::InitLights()
{
	initClearColor();
	initDirectionalLight();
	createPointLights();
	initSpotLight();
}

void LightManager::createShaders()
{
	// build and compile our shader zprogram
	// ------------------------------------
	_lampShader = new  Shader("shader_lamp.vs", "shader_lamp.fs");
	SimpleDepthShader = new Shader("shader_depth.vs", "shader_depth.fs");
	_debugDepthQuad = new Shader("shader_depth_debug.vs", "shader_depth_debug.fs");
}

void LightManager::createDepthMap()
{
	// configure depth map FBO
	// -----------------------

	glGenFramebuffers(1, &_depthMapFBO);
	// create depth texture
	glGenTextures(1, &DepthMap);
	glBindTexture(GL_TEXTURE_2D, DepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//debug

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &_quadVBO);
	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void LightManager::configureShaders()
{
	_debugDepthQuad->use();
	_debugDepthQuad->setInt("depthMap", 0);
}

void LightManager::initClearColor()
{
	ClearColor = ImColor(25, 25, 25);
}

void LightManager::initDirectionalLight()
{
	if (LightDirectionalLight != nullptr) 
	{
		LightDirectionalLight->enabled = true;
		LightDirectionalLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		LightDirectionalLight->ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		LightDirectionalLight->diffuse = glm::vec3(0.15f, 0.15f, 0.15f);
		LightDirectionalLight->specular = glm::vec3(0.15f, 0.15f, 0.15f);
		LightDirectionalLight->position = glm::vec3(-2.75f, 10.0f, 0.0f);
	}
}

void LightManager::createPointLights()
{
	if (LightPointLights != nullptr) 
	{
		for (int i = 0; i < 4; i++)
		{
			PointLight *newPointLight = new PointLight();
			LightPointLights->push_back(newPointLight);
			initPointLight(i);
		}
	}
}

void LightManager::initPointLight(int index)
{
	LightPointLights->at(index)->enabled = true;
	LightPointLights->at(index)->visiable = true;
	LightPointLights->at(index)->ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	LightPointLights->at(index)->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	LightPointLights->at(index)->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	LightPointLights->at(index)->constant = 1.0f;
	LightPointLights->at(index)->linear = 0.09f;
	LightPointLights->at(index)->quadratic = 0.032f;

	switch (index)
	{
	case 0:
		LightPointLights->at(index)->position = glm::vec3(0.7f, 0.2f, 2.0f);
		break;
	case 1:
		LightPointLights->at(index)->position = glm::vec3(2.3f, 3.3f, -4.0f);
		break;
	case 2:
		LightPointLights->at(index)->position = glm::vec3(-4.0f, 2.0f, -10.0f);
		break;
	case 3:
		LightPointLights->at(index)->position = glm::vec3(0.0f, 0.0f, -3.0f);
		break;
	default:
		break;
	}
	
}

void LightManager::resetPointLights()
{
	for (int i = 0; i < LightPointLights->size(); i++)
	{
		initPointLight(i);

	}
}

void LightManager::initSpotLight()
{
	if (LightSpotLight != nullptr)
	{
		LightSpotLight->enabled = true;
		LightSpotLight->position = _camera->Position;
		LightSpotLight->direction = _camera->Front;
		LightSpotLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		LightSpotLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		LightSpotLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
		LightSpotLight->constant = 1.0f;
		LightSpotLight->linear = 0.09f;
		LightSpotLight->quadratic = 0.032f;
		LightSpotLight->cutOff = glm::cos(glm::radians(12.5f));
		LightSpotLight->outerCutOff = glm::cos(glm::radians(15.0f));
	}
}


void LightManager::UpdateSpotLight()
{
	if (LightSpotLight != nullptr && LightSpotLight->enabled)
	{
		LightSpotLight->position = _camera->Position;
		LightSpotLight->direction = _camera->Front;
	}
}


void LightManager::RenderLights()
{
	//updates
	UpdateSpotLight();
	SetThemePreset(LightPresets);

	glm::mat4 model;
	// draw the lamp object(s)
	_lampShader->use();
	_lampShader->setMat4("projection", _camera->GetProjectionMatrix());
	_lampShader->setMat4("view", _camera->GetViewMatrix());

	// we now draw as many light bulbs as we have point lights.
	for (unsigned int i = 0; i < LightPointLights->size(); i++)
	{
		if (LightPointLights->at(i)->enabled && LightPointLights->at(i)->visiable)
		{
			// render the loaded model
			glm::mat4 model;
			model = glm::translate(model, LightPointLights->at(i)->position); 
			model = glm::scale(model, glm::vec3(0.08f));
			_lampShader->setMat4("model", model);
			_lampShader->setVec3("colorTint", LightPointLights->at(i)->diffuse);
			_lampModel->Draw(*_lampShader);
		}
	}
}

void LightManager::SetThemePreset(int presetIndex)
{
	if (_lastPreset != presetIndex) {
		_lastPreset = presetIndex;
		glm::vec3 *pointLightColors = new glm::vec3[4];
		switch (presetIndex)
		{
		case 0:
			initClearColor();
			initDirectionalLight();
			resetPointLights();
			initSpotLight();
			break;
		case 1:
			ClearColor = ImVec4(0.75f,0.52f,0.3f,1.0f);
			//directional light
			LightDirectionalLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
			LightDirectionalLight->ambient = glm::vec3(0.3f, 0.24f, 0.14f);
			LightDirectionalLight->diffuse = glm::vec3(0.7f, 0.42f, 0.26f);
			LightDirectionalLight->specular = glm::vec3(0.5f, 0.5f, 0.5f);
			//point lights
			pointLightColors[0] = glm::vec3(1.0f, 0.6f, 0.0f);
			pointLightColors[1] = glm::vec3(1.0f, 0.0f, 0.0f);
			pointLightColors[2] = glm::vec3(1.0f, 1.0, 0.0);
			pointLightColors[3] = glm::vec3(0.2f, 0.2f, 1.0f);
			for (int i = 0; i < LightPointLights->size(); i++)
			{
				LightPointLights->at(i)->enabled = true;
				//LightPointLights->at(i)->position = pointLightColors[i];
				LightPointLights->at(i)->ambient = pointLightColors[i] * 0.1f;
				LightPointLights->at(i)->diffuse = pointLightColors[i];
				LightPointLights->at(i)->specular = pointLightColors[i];
				LightPointLights->at(i)->constant = 1.0f;
				LightPointLights->at(i)->linear = 0.09f;
				LightPointLights->at(i)->quadratic = 0.032f;
			}
			//spot light
			LightSpotLight->enabled = true;
			LightSpotLight->position = _camera->Position;
			LightSpotLight->direction = _camera->Front;
			LightSpotLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			LightSpotLight->diffuse = glm::vec3(0.8f, 0.8f, 0.0f);
			LightSpotLight->specular = glm::vec3(0.8f, 0.8f, 0.0f);
			LightSpotLight->constant = 1.0f;
			LightSpotLight->linear = 0.09f;
			LightSpotLight->quadratic = 0.032f;
			LightSpotLight->cutOff = glm::cos(glm::radians(12.5f));
			LightSpotLight->outerCutOff = glm::cos(glm::radians(15.0f));
			
			break;
		case 2:
			ClearColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
			//directional light
			LightDirectionalLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
			LightDirectionalLight->ambient = glm::vec3(0.05f, 0.05f, 0.1f);
			LightDirectionalLight->diffuse = glm::vec3(0.2f, 0.2f, 0.7f);
			LightDirectionalLight->specular = glm::vec3(0.7f, 0.7f, 0.7f);
			//point lights
			pointLightColors[0] = glm::vec3(0.2f, 0.2f, 0.6f);
			pointLightColors[1] = glm::vec3(0.3f, 0.3f, 0.7f);
			pointLightColors[2] = glm::vec3(0.0f, 0.0f, 0.3f);
			pointLightColors[3] = glm::vec3(0.4f, 0.4f, 0.4f);
			for (int i = 0; i < LightPointLights->size(); i++)
			{
				LightPointLights->at(i)->enabled = true;
				//LightPointLights->at(i)->position = pointLightColors[i];
				LightPointLights->at(i)->ambient = pointLightColors[i] * 0.1f;
				LightPointLights->at(i)->diffuse = pointLightColors[i];
				LightPointLights->at(i)->specular = pointLightColors[i];
				LightPointLights->at(i)->constant = 1.0f;
				LightPointLights->at(i)->linear = 0.09f;
				LightPointLights->at(i)->quadratic = 0.032f;
			}
			//spot light
			LightSpotLight->enabled = true;
			LightSpotLight->position = _camera->Position;
			LightSpotLight->direction = _camera->Front;
			LightSpotLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			LightSpotLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			LightSpotLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
			LightSpotLight->constant = 1.0f;
			LightSpotLight->linear = 0.09f;
			LightSpotLight->quadratic = 0.032f;
			LightSpotLight->cutOff = glm::cos(glm::radians(12.5f));
			LightSpotLight->outerCutOff = glm::cos(glm::radians(15.0f));
			break;
		case 3:
			ClearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			//directional light
			LightDirectionalLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
			LightDirectionalLight->ambient = glm::vec3(0.0f, 0.00f, 0.0f);
			LightDirectionalLight->diffuse = glm::vec3(0.05f, 0.05f, 0.05f);
			LightDirectionalLight->specular = glm::vec3(0.2f, 0.2f, 0.2f);
			//point lights
			pointLightColors[0] = glm::vec3(0.1f, 0.1f, 0.1f);
			pointLightColors[1] = glm::vec3(0.1f, 0.1f, 0.1f);
			pointLightColors[2] = glm::vec3(0.1f, 0.1f, 0.1f);
			pointLightColors[3] = glm::vec3(0.3f, 0.1f, 0.1f);
			for (int i = 0; i < LightPointLights->size(); i++)
			{
				LightPointLights->at(i)->enabled = true;
				//LightPointLights->at(i)->position = pointLightColors[i];
				LightPointLights->at(i)->ambient = pointLightColors[i] * 0.1f;
				LightPointLights->at(i)->diffuse = pointLightColors[i];
				LightPointLights->at(i)->specular = pointLightColors[i];
				LightPointLights->at(i)->constant = 1.0f;
				LightPointLights->at(i)->linear = 0.14f;
				LightPointLights->at(i)->quadratic = 0.07f;
			}
			//spot light
			LightSpotLight->enabled = true;
			LightSpotLight->position = _camera->Position;
			LightSpotLight->direction = _camera->Front;
			LightSpotLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			LightSpotLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			LightSpotLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
			LightSpotLight->constant = 1.0f;
			LightSpotLight->linear = 0.09f;
			LightSpotLight->quadratic = 0.032f;
			LightSpotLight->cutOff = glm::cos(glm::radians(10.0f));
			LightSpotLight->outerCutOff = glm::cos(glm::radians(15.0f));
			break;
		case 4:
			ClearColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
			//directional light
			LightDirectionalLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
			LightDirectionalLight->ambient = glm::vec3(0.5f, 0.5f, 0.5f);
			LightDirectionalLight->diffuse = glm::vec3(0.15f, 0.15f, 0.15f);
			LightDirectionalLight->specular = glm::vec3(0.15f, 0.15f, 0.15f);
			//point lights
			pointLightColors[0] = glm::vec3(0.4f, 0.7f, 0.1f);
			pointLightColors[1] = glm::vec3(0.4f, 0.7f, 0.1f);
			pointLightColors[2] = glm::vec3(0.4f, 0.7f, 0.1f);
			pointLightColors[3] = glm::vec3(0.4f, 0.7f, 0.1f);
			for (int i = 0; i < LightPointLights->size(); i++)
			{
				LightPointLights->at(i)->enabled = true;
				//LightPointLights->at(i)->position = pointLightColors[i];
				LightPointLights->at(i)->ambient = pointLightColors[i] * 0.1f;
				LightPointLights->at(i)->diffuse = pointLightColors[i];
				LightPointLights->at(i)->specular = pointLightColors[i];
				LightPointLights->at(i)->constant = 1.0f;
				LightPointLights->at(i)->linear = 0.07f;
				LightPointLights->at(i)->quadratic = 0.017f;
			}
			//spot light
			LightSpotLight->enabled = true;
			LightSpotLight->position = _camera->Position;
			LightSpotLight->direction = _camera->Front;
			LightSpotLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			LightSpotLight->diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
			LightSpotLight->specular = glm::vec3(0.0f, 1.0f, 0.0f);
			LightSpotLight->constant = 1.0f;
			LightSpotLight->linear = 0.07f;
			LightSpotLight->quadratic = 0.017f;
			LightSpotLight->cutOff = glm::cos(glm::radians(7.0f));
			LightSpotLight->outerCutOff = glm::cos(glm::radians(10.0f));
			break;
		default:
			break;
		}
	delete pointLightColors;
	}
}

void LightManager::RenderShadwoMapToTextureBegin()
{
	// render scene from light's point of view
	SimpleDepthShader->use();
	SimpleDepthShader->setMat4("lightSpaceMatrix", GetLightSpaceMatrix());

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void LightManager::RenderShadwoMapToTextureEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightManager::RenderShadowMappDebug()
{
	if (EnableShadowMapDebug) {
		_debugDepthQuad->use();
		_debugDepthQuad->setFloat("near_plane", NearShadowPlane);
		_debugDepthQuad->setFloat("far_plane", FarShadowPlane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DepthMap);

		glBindVertexArray(_quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}

glm::mat4 LightManager::GetLightSpaceMatrix()
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-ShadowRegion, ShadowRegion, -ShadowRegion, ShadowRegion, NearShadowPlane, FarShadowPlane);
	lightView = glm::lookAt(LightDirectionalLight->position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

