#include "prism.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;
unsigned int  Prism::VAO;
unsigned int  Prism::VBO;
int  Prism::length;

Prism::Prism(glm::vec3 pos, float p, float y, float r, float x_c, float y_c, float z_c) :Object3D(pos, p, y, r, x_c, y_c, z_c)
{
	len = length;

}


