#include "cube.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


using namespace std;
unsigned int  Cube::VAO;
unsigned int  Cube::VBO;
int  Cube::length;

Cube::Cube(glm::vec3 pos, float p, float y, float r, float x_c, float y_c, float z_c):Object3D(pos,p,y,r,x_c,y_c,z_c)
{
	len = length;

}
