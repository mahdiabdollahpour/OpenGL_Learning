#pragma once
#include "object3D.h"
#include <iostream>
using namespace std;
class Pyramid :
	public Object3D
{
public:
	static unsigned int VBO, VAO;
	static int length;
	Pyramid(glm::vec3 pos, float p, float y, float r, float x_c, float y_c, float z_c);
	~Pyramid() {
		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
	};
	virtual unsigned int getVAO() {

		return Pyramid::VAO;

	};
	static void setShapes() {
		float pyramid_V[] = {

			0.0f, 1.0f, 0.0f,  0.0f, 0.0f,//A
			1.0f,  0.0f, 1.0f,  1.0f, 1.0f,//C
			1.0f, 0.0f, -1.0f,  1.0f, 0.0f,//B
			
			0.0f, 1.0f, 0.0f,   1.0f, 1.0f,//A
			-1.0f, 0.0f, 1.0f,  0.0f, 0.0f,//D
			1.0f,  0.0f, 1.0f,  0.0f, 1.0f,//C

			0.0f,1.0f, 0.0f,  0.0f, 0.0f,//A
			-1.0f,  0.0f,  -1.0f,  1.0f, 1.0f,//E
			-1.0f, 0.0f, 1.0f,   1.0f, 0.0f,//D

			0.0f,1.0f, 0.0f,  1.0f, 1.0f,//A
			1.0f, 0.0f, -1.0f, 0.0f, 0.0f,//B
			-1.0f,  0.0f,  -1.0f, 0.0f, 1.0f,//E
			
			
			1.0f, 0.0f, -1.0f, 1.0f, 1.0f,//B
			-1.0f, 0.0f, 1.0f,  0.0f, 1.0f,//D
			-1.0f,  0.0f,  -1.0f,  1.0f, 0.0f,//E
		

			-1.0f, 0.0f, 1.0f,  1.0f, 0.0f,//D
			1.0f, 0.0f, -1.0f,  0.0f, 0.0f,//B
			1.0f,  0.0f, 1.0f,  0.0f, 1.0f,//C
			
		};
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_V), pyramid_V, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		Pyramid::VAO = VAO;
		Pyramid::VBO = VBO;
		Pyramid::length = sizeof(pyramid_V) / (sizeof(float) * 5);


	}
};

