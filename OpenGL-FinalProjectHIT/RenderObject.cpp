#include "RenderObject.h"

RenderObject::RenderObject(Camera* camera)
{
	_camera = camera;
	createShader();
	ObjectsVec = new vector<Object*>();
}

RenderObject::~RenderObject()
{
	delete(_objectShader);
	for each (Object* obj in *ObjectsVec)
	{
		delete(obj);
	}
	delete(ObjectsVec);
}

void RenderObject::createShader()
{
	// build and compile our shader zprogram
	// ------------------------------------
	_objectShader = new Shader("shader_object.vs", "shader_object.fs");

	// shader configuration
	// --------------------
	_objectShader->use();

	_objectShader->setInt("material.diffuse", 0);
	_objectShader->setInt("material.specular", 1);
	_objectShader->setInt("shadowMap", 2);
	_objectShader->setInt("material.shininess", 32);
}


void RenderObject::render()
{
	// be sure to activate shader when setting uniforms/drawing objects
	_objectShader->use();

	// view/projection transformations
	_objectShader->setMat4("projection", _camera->GetProjectionMatrix());
	_objectShader->setMat4("view", _camera->GetViewMatrix());

	_objectShader->setVec3("viewPos", _camera->Position);

	//sending light info to the shader
	setDirectionalLight();
	setPointLights();
	setSpotLight();

	//sending shadow info to the shader
	_objectShader->setMat4("lightSpaceMatrix", Lights->GetLightSpaceMatrix());
	_objectShader->setBool("enableShadows", Lights->EnableShadows);


}


void RenderObject::setDirectionalLight()
{
	if (Lights->LightDirectionalLight != nullptr)
	{
		_objectShader->setVec3("dirLight.direction", Lights->LightDirectionalLight->direction);
		if (Lights->LightDirectionalLight->enabled)
		{
			_objectShader->setVec3("dirLight.ambient", Lights->LightDirectionalLight->ambient);
			_objectShader->setVec3("dirLight.diffuse", Lights->LightDirectionalLight->diffuse);
			_objectShader->setVec3("dirLight.specular", Lights->LightDirectionalLight->specular);
		}
		else
		{
			_objectShader->setVec3("dirLight.ambient", glm::vec3(0));
			_objectShader->setVec3("dirLight.diffuse", glm::vec3(0));
			_objectShader->setVec3("dirLight.specular", glm::vec3(0));
		}

	}
}

void RenderObject::setPointLights()
{
	if (Lights->LightPointLights != nullptr)
	{
		for (int i = 0; i < Lights->LightPointLights->size(); i++)
		{
			setPointLight(i);
		}
	}
}

void RenderObject::setPointLight(int index)
{
	std::string str = "pointLights[";
	str = str.append(std::to_string(index));
	str.append("]");
	std::string subComponnet = str + ".position";
	_objectShader->setVec3(subComponnet, Lights->LightPointLights->at(index)->position);
	if (Lights->LightPointLights->at(index)->enabled)
	{
		subComponnet = str + ".ambient";
		_objectShader->setVec3(subComponnet, Lights->LightPointLights->at(index)->ambient);
		subComponnet = str + ".diffuse";
		_objectShader->setVec3(subComponnet, Lights->LightPointLights->at(index)->diffuse);
		subComponnet = str + ".specular";
		_objectShader->setVec3(subComponnet, Lights->LightPointLights->at(index)->specular);
	}
	else
	{
		subComponnet = str + ".ambient";
		_objectShader->setVec3(subComponnet, glm::vec3(0));
		subComponnet = str + ".diffuse";
		_objectShader->setVec3(subComponnet, glm::vec3(0));
		subComponnet = str + ".specular";
		_objectShader->setVec3(subComponnet, glm::vec3(0));
	}


	subComponnet = str + ".constant";
	_objectShader->setFloat(subComponnet, Lights->LightPointLights->at(index)->constant);
	subComponnet = str + ".linear";
	_objectShader->setFloat(subComponnet, Lights->LightPointLights->at(index)->linear);
	subComponnet = str + ".quadratic";
	_objectShader->setFloat(subComponnet, Lights->LightPointLights->at(index)->quadratic);
}

void RenderObject::setSpotLight()
{
	if (Lights->LightSpotLight != nullptr) {
		std::string str = "spotLight";
		std::string subComponnet = str + ".position";
		_objectShader->setVec3(subComponnet, Lights->LightSpotLight->position);
		subComponnet = str + ".direction";
		_objectShader->setVec3(subComponnet, Lights->LightSpotLight->direction);
		subComponnet = str + ".constant";
		_objectShader->setFloat(subComponnet, Lights->LightSpotLight->constant);
		subComponnet = str + ".linear";
		_objectShader->setFloat(subComponnet, Lights->LightSpotLight->linear);
		subComponnet = str + ".quadratic";
		_objectShader->setFloat(subComponnet, Lights->LightSpotLight->quadratic);
		subComponnet = str + ".cutOff";
		_objectShader->setFloat(subComponnet, Lights->LightSpotLight->cutOff);
		subComponnet = str + ".outerCutOff";
		_objectShader->setFloat(subComponnet, Lights->LightSpotLight->outerCutOff);

		if (Lights->LightSpotLight->enabled)
		{
			subComponnet = str + ".ambient";
			_objectShader->setVec3(subComponnet, Lights->LightSpotLight->ambient);
			subComponnet = str + ".diffuse";
			_objectShader->setVec3(subComponnet, Lights->LightSpotLight->diffuse);
			subComponnet = str + ".specular";
			_objectShader->setVec3(subComponnet, Lights->LightSpotLight->specular);
		}
		else
		{
			_objectShader->setVec3(subComponnet, glm::vec3(0));
			subComponnet = str + ".diffuse";
			_objectShader->setVec3(subComponnet, glm::vec3(0));
			subComponnet = str + ".specular";
			_objectShader->setVec3(subComponnet, glm::vec3(0));
			subComponnet = str + ".constant";
		}
	}
}
