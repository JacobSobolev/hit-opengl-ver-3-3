#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

struct Material {
	unsigned int diffuseMap;
	unsigned int specularMap;
	float shininess;
};

#endif

