#pragma once
#include "object3D.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;
class Cube :
	public Object3D
{
public:
	static unsigned int VBO, VAO;
	static int length;
	Cube(glm::vec3 pos,float p,float y, float r, float x_c, float y_c, float z_c);
	~Cube() {
	};
	virtual unsigned int getVAO() {

		return Cube::VAO;

	};

	static void setShapes() {
		float cube_v[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right  
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
			 

			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left   
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			             
											  // Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
  

		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left 
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	       
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left  
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		    

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		
		// Right face
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right  
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
	

		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		
		// Bottom face          
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left 
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		   

		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	  
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left 
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	 
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left   
		};


		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_v), cube_v, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		Cube::VAO = VAO;
		Cube::VBO = VBO;
		Cube::length = sizeof(cube_v) / (sizeof(float) * 5);
		cout << "SETTING cube " << VAO << endl;
	}
};

