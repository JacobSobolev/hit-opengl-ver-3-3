#include "FloorManager.h"



FloorManager::FloorManager(Camera* camera) : RenderObject(camera)
{

	createObject();
	createTexture();
	initFloor();
}

void FloorManager::createObject()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float planeVertices[] = {
		// positions            // normals         // texcoords
		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};
	// plane VAO
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void FloorManager::createTexture()
{
	// load textures
	// -------------
	_diffuseMap = TextureLoader::Load("textures/wood_tex.png", "", false);
	_specularMap = TextureLoader::Load("textures/wood_spec.png", "", false);

	
}

void FloorManager::initFloor()
{
	Object* newObject = new Object();
	glm::mat4 model;
	float angle = 0;
	newObject->positionInit = glm::vec3(0,-1.0f,0);
	newObject->scaleInit = glm::vec3(2.0f,1.0f,2.0f);
	newObject->rotationInit = glm::vec3(1.0f, 0.3f, 0.5f);
	newObject->rotationAngleInit = glm::radians(angle);

	newObject->position = glm::vec3(0);
	newObject->scale = glm::vec3(1);
	newObject->rotation = glm::vec3(1);
	newObject->rotationAngle = glm::radians(0.0f);

	newObject->material.diffuseMap = _diffuseMap;
	newObject->material.specularMap = _specularMap;
	newObject->material.shininess = 32.0f;

	newObject->animationEnabled = true;
	newObject->visiable = true;
	newObject->blin = true;

	ObjectsVec->push_back(newObject);
}


FloorManager::~FloorManager()
{
	glDeleteVertexArrays(1, &_VBO);
	glDeleteBuffers(1, &_VAO);
}

void FloorManager::RenderFloor()
{
	RenderObject::render();

	_objectShader->use();

	glBindVertexArray(_VAO);
	for (int i = 0; i < ObjectsVec->size(); i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		if (ObjectsVec->at(i)->visiable)
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


			_objectShader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	glBindVertexArray(0);
}

void FloorManager::RenderFloorForShadowMap()
{
	Lights->SimpleDepthShader->use();
	glBindVertexArray(_VAO);
	for (int i = 0; i < ObjectsVec->size(); i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		if (ObjectsVec->at(i)->visiable)
		{
			glm::mat4 model;

			model = glm::translate(model, ObjectsVec->at(i)->positionInit);
			model = glm::scale(model, ObjectsVec->at(i)->scaleInit);
			model = glm::rotate(model, ObjectsVec->at(i)->rotationAngleInit, ObjectsVec->at(i)->rotationInit);

			model = glm::translate(model, ObjectsVec->at(i)->position);
			model = glm::scale(model, ObjectsVec->at(i)->scale);
			model = glm::rotate(model, ObjectsVec->at(i)->rotationAngle, ObjectsVec->at(i)->rotation);

			Lights->SimpleDepthShader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	glBindVertexArray(0);
}

