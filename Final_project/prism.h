#pragma once
#include "object3D.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;
class Prism :
	public Object3D
{
public:
	static unsigned int VBO, VAO;
	static int length;
	Prism(glm::vec3 pos, float p, float y, float r, float x_c, float y_c, float z_c);
	~Prism() {
	};
	virtual unsigned int getVAO() {
		return Prism::VAO;
	};

	static void setShapes() {
		float prism_v[] = {
			-1.0f, 1.0f, -1.0f,  0.0f, 0.0f,//A
			0.0f,  1.0f, 1.0f,  1.0f, 1.0f,//C
			1.0f, 1.0f, -1.0f,  1.0f, 0.0f,//B

			-1.0f, -1.0f, -1.0f,  1.0f, 1.0f,//D
			1.0f, -1.0f, -1.0f,   0.0f, 1.0f,//E
			0.0f,  -1.0f, 1.0f,  0.0f, 0.0f,//F
			

			-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,//D
			0.0f,  1.0f, 1.0f,  1.0f, 1.0f,//C
			-1.0f, 1.0f, -1.0f,  1.0f, 0.0f,//A

			-1.0f, -1.0f, -1.0f,  1.0f, 1.0f,//D
			0.0f,  -1.0f, 1.0f,  0.0f, 0.0f,//F
			0.0f,  1.0f, 1.0f,   0.0f, 1.0f,//C

			1.0f, -1.0f, -1.0f,  1.0f, 0.0f,//E
			1.0f, 1.0f, -1.0f,  0.0f, 1.0f,//B
			0.0f,  1.0f, 1.0f, 1.0f, 1.0f,//C

			0.0f,  -1.0f, 1.0f,  1.0f, 0.0f,//F
			1.0f, -1.0f, -1.0f,  0.0f, 1.0f,//E
			0.0f,  1.0f, 1.0f,  0.0f, 0.0f,//C

			-1.0f, -1.0f, -1.0f,  1.0f, 0.0f,//D
			1.0f, 1.0f, -1.0f, 1.0f, 1.0f,//B
			1.0f, -1.0f, -1.0f,  0.0f, 1.0f,//E

			-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,//D
			-1.0f, 1.0f, -1.0f,  1.0f, 0.0f,//A
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f,//B
		

		};


		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(prism_v), prism_v, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		Prism::VAO = VAO;
		Prism::VBO = VBO;
		Prism::length = sizeof(prism_v) / (sizeof(float) * 5);
		cout << "SETTING Prism " << VAO << endl;
	}
};

