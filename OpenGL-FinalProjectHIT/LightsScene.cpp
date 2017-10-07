#include "LightsScene.h"

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

LightsScene::LightsScene(unsigned int screenWidth, unsigned int screenHeight, Camera *camera)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_camera = camera;

	// build and compile our shader zprogram
	// ------------------------------------
	_lampShader = new  Shader("shader_lamp.vs", "shader_lamp.fs");


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};


	/*_cubePositions = cubePositions;



	_pointLightPositions = pointLightPositions;*/

	// first, configure the cube's VAO (and VBO)
	glGenBuffers(1, &_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)

	glGenVertexArrays(1, &_lightVAO);
	glBindVertexArray(_lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}


LightsScene::~LightsScene()
{
}

void LightsScene::RenderScene() 
{
	//// be sure to activate shader when setting uniforms/drawing objects
	//_lightingShader->use();
	//_lightingShader->setVec3("viewPos", _camera->Position);
	//_lightingShader->setFloat("material.shininess", 32.0f);

	///*
	//Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	//the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	//by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	//by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	//*/
	//// directional light
	//_lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//_lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//_lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//_lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	//// point light 1
	//_lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
	//_lightingShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	//_lightingShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	//_lightingShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	//_lightingShader->setFloat("pointLights[0].constant", 1.0f);
	//_lightingShader->setFloat("pointLights[0].linear", 0.09);
	//_lightingShader->setFloat("pointLights[0].quadratic", 0.032);
	//// point light 2
	//_lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
	//_lightingShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	//_lightingShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	//_lightingShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	//_lightingShader->setFloat("pointLights[1].constant", 1.0f);
	//_lightingShader->setFloat("pointLights[1].linear", 0.09);
	//_lightingShader->setFloat("pointLights[1].quadratic", 0.032);
	//// point light 3
	//_lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
	//_lightingShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	//_lightingShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	//_lightingShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	//_lightingShader->setFloat("pointLights[2].constant", 1.0f);
	//_lightingShader->setFloat("pointLights[2].linear", 0.09);
	//_lightingShader->setFloat("pointLights[2].quadratic", 0.032);
	//// point light 4
	//_lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
	//_lightingShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	//_lightingShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	//_lightingShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	//_lightingShader->setFloat("pointLights[3].constant", 1.0f);
	//_lightingShader->setFloat("pointLights[3].linear", 0.09);
	//_lightingShader->setFloat("pointLights[3].quadratic", 0.032);
	//// spotLight
	//_lightingShader->setVec3("spotLight.position", _camera->Position);
	//_lightingShader->setVec3("spotLight.direction", _camera->Front);
	//_lightingShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	//_lightingShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	//_lightingShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	//_lightingShader->setFloat("spotLight.constant", 1.0f);
	//_lightingShader->setFloat("spotLight.linear", 0.09);
	//_lightingShader->setFloat("spotLight.quadratic", 0.032);
	//_lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	//_lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	//// view/projection transformations
	//glm::mat4 projection = glm::perspective(glm::radians(_camera->Zoom), (float)_screenWidth / (float)_screenHeight, 0.1f, 100.0f);
	//glm::mat4 view = _camera->GetViewMatrix();
	//_lightingShader->setMat4("projection", projection);
	//_lightingShader->setMat4("view", view);

	//// world transformation
	//glm::mat4 model;
	//_lightingShader->setMat4("model", model);


	//// also draw the lamp object(s)
	//_lampShader->use();
	//_lampShader->setMat4("projection", projection);
	//_lampShader->setMat4("view", view);

	//// we now draw as many light bulbs as we have point lights.
	//glBindVertexArray(_lightVAO);
	//for (unsigned int i = 0; i < 4; i++)
	//{
	//	model = glm::mat4();
	//	model = glm::translate(model, pointLightPositions[i]);
	//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
	//	_lampShader->setMat4("model", model);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//}
}
