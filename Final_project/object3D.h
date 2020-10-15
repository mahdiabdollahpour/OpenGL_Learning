#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;
class Object3D
{
public:

	static unsigned int texture;
	int textureInt;
	glm::vec3 position;
	glm::mat4 getModel();
	virtual unsigned int getVAO() {
		return 0;

	};
	float pitch;
	int len;
	float  yaw;
	float roll;
	float x_coef;
	float y_coef;
	float z_coef;
	Object3D::Object3D(glm::vec3 pos, float p, float y, float r, float x_c, float y_c, float z_c);

	~Object3D() {
		
	};

	//unsigned int texture;
};

