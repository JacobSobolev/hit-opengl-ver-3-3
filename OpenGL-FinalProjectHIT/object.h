#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "Material.h"

struct Object {
	glm::vec3 positionInit;
	glm::vec3 scaleInit;
	glm::vec3 rotationInit;
	float rotationAngleInit;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	float rotationAngle;

	Material material;

	bool animationEnabled;
	bool visiable;
	bool blin;
};

#endif