#include "object3D.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Object3D::Object3D(glm::vec3 pos, float p, float y, float r, float x_c, float y_c, float z_c)
{
	position = pos;
	//rotationAxis = rotation_axis;
	x_coef = x_c;
	y_coef = y_c;
	z_coef = z_c;
	pitch = p;
	yaw = y;
	roll = r;
}
 


glm::mat4 Object3D::getModel() {
	glm::mat4 model = glm::mat4(1.0f);
	//cout << position[0] << " " << position[1]<< " " << position[2] << endl;
	model = glm::translate(model, position);

	model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(x_coef, y_coef, z_coef));
	return model;
}


//Object3D::~Object3D();