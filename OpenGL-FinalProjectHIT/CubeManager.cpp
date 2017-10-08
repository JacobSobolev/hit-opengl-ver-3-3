#include "CubeManager.h"


CubeManager::CubeManager(Camera* camera) : RenderObject(camera)
{
	_lastTextureIndex = 0;
	TextureItem = 0;
	AnimateCubes = true;
	VisiableCubes = true;
	createObject();
	createTexture();
	initCubes();
}


void CubeManager::createObject() 
{
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



	// first, configure the cube's VAO (and VBO)
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(_VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void CubeManager::createTexture()
{
	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	_diffuseMap1 = TextureLoader::Load("textures/box1_tex.jpg", "", false);
	_diffuseMap2 = TextureLoader::Load("textures/box2_tex.jpg", "", false);
	_diffuseMap3 = TextureLoader::Load("textures/box3_tex.jpg", "", false);
	_specularMap1 = TextureLoader::Load("textures/box1_spec.jpg", "", false);
	_specularMap2 = TextureLoader::Load("textures/box2_spec.jpg", "", false);
	_specularMap3 = TextureLoader::Load("textures/box3_spec.jpg", "", false);
}


CubeManager::~CubeManager()
{
	glDeleteVertexArrays(1, &_VBO);
	glDeleteBuffers(1, &_VAO);

}


void CubeManager::initCubes()
{
	// positions all containers
	int numOfCubes = 10;
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, 2.2f, -2.5f),
		glm::vec3(-3.8f, 2.0f, -12.3f),
		glm::vec3(2.4f, 0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, 4.0f, -2.5f),
	};


	for (int i = 0; i < numOfCubes; i++)
	{
		Object* newObject = new Object();
		glm::mat4 model;
		float angle = 20.0f * i;
		newObject->positionInit = cubePositions[i];
		newObject->scaleInit = glm::vec3(1);
		newObject->rotationInit = glm::vec3(1.0f, 0.3f, 0.5f);
		newObject->rotationAngleInit = glm::radians(angle);

		newObject->position = glm::vec3(0);
		newObject->scale = glm::vec3(1);
		newObject->rotation = glm::vec3(1.0f, 0.3f, 0.5f);
		newObject->rotationAngle = glm::radians(0.0f);

		newObject->material.diffuseMap = _diffuseMap1;
		newObject->material.specularMap = _specularMap1;
		newObject->material.shininess = 32.0f;

		newObject->animationEnabled = true;
		newObject->visiable = true;
		newObject->blin = true;

		ObjectsVec->push_back(newObject);
	}
}

void CubeManager::RenderCubes(float dt)
{
	RenderObject::render();

	SetTextureToAll(TextureItem);

	// render containers
	glBindVertexArray(_VAO);
	for (int i = 0; i < ObjectsVec->size(); i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		if (VisiableCubes && ObjectsVec->at(i)->visiable )
		{
			_objectShader->setFloat("material.shininess", ObjectsVec->at(i)->material.shininess);
			_objectShader->setBool("blinn", ObjectsVec->at(i)->blin);
			// bind diffuse map
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ObjectsVec->at(i)->material.diffuseMap);
			// bind specular map
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, ObjectsVec->at(i)->material.specularMap);
			//shadow map
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, Lights->DepthMap);

			glm::mat4 model;

			model = glm::translate(model, ObjectsVec->at(i)->positionInit);
			model = glm::scale(model, ObjectsVec->at(i)->scaleInit);
			model = glm::rotate(model, ObjectsVec->at(i)->rotationAngleInit, ObjectsVec->at(i)->rotationInit);

			model = glm::translate(model, ObjectsVec->at(i)->position);
			model = glm::scale(model, ObjectsVec->at(i)->scale);
			model = glm::rotate(model, ObjectsVec->at(i)->rotationAngle, ObjectsVec->at(i)->rotation);


			if (AnimateCubes && ObjectsVec->at(i)->animationEnabled)
			{
				ObjectsVec->at(i)->rotationAngle += dt * glm::radians(360.0f) * 0.5;
			}


			_objectShader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	glBindVertexArray(0);
}

void CubeManager::RenderCubesForShadowMap()
{
	Lights->SimpleDepthShader->use();
	glBindVertexArray(_VAO);
	for (int i = 0; i < ObjectsVec->size(); i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		if (VisiableCubes && ObjectsVec->at(i)->visiable)
		{
			glm::mat4 model;

			model = glm::translate(model, ObjectsVec->at(i)->positionInit);
			model = glm::scale(model, ObjectsVec->at(i)->scaleInit);
			model = glm::rotate(model, ObjectsVec->at(i)->rotationAngleInit, ObjectsVec->at(i)->rotationInit);

			model = glm::translate(model, ObjectsVec->at(i)->position);
			model = glm::scale(model, ObjectsVec->at(i)->scale);
			model = glm::rotate(model, ObjectsVec->at(i)->rotationAngle, ObjectsVec->at(i)->rotation);

			Lights->SimpleDepthShader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	glBindVertexArray(0);
}

void CubeManager::SetTextureToAll(int textureIndex)
{
	if (_lastTextureIndex != textureIndex) {
		_lastTextureIndex = textureIndex;
		unsigned int diffuseToSet;
		unsigned int specularToSet;
		switch (textureIndex)
		{
			case 0:
				diffuseToSet = _diffuseMap1;
				specularToSet = _specularMap1;
				break;
			case 1:
				diffuseToSet = _diffuseMap2;
				specularToSet = _specularMap2;
				break;
			case 2:
				diffuseToSet = _diffuseMap3;
				specularToSet = _specularMap3;
				break;
			default:
				break;
		}
		for (int i = 0; i < ObjectsVec->size(); i++)
		{
			ObjectsVec->at(i)->material.diffuseMap = diffuseToSet;
			ObjectsVec->at(i)->material.specularMap = specularToSet;
			
		}
	}
}

void CubeManager::SetTextureToCube(int textureIndex, int cubeIndex)
{
	unsigned int diffuseToSet;
	unsigned int specularToSet;
	switch (textureIndex)
	{
		case 0:
			diffuseToSet = _diffuseMap1;
			specularToSet = _specularMap1;
			break;
		case 1:
			diffuseToSet = _diffuseMap2;
			specularToSet = _specularMap2;
			break;
		case 2:
			diffuseToSet = _diffuseMap3;
			specularToSet = _specularMap3;
			break;
		default:
			break;
	}
	ObjectsVec->at(cubeIndex)->material.diffuseMap = diffuseToSet;
	ObjectsVec->at(cubeIndex)->material.specularMap = specularToSet;
}
