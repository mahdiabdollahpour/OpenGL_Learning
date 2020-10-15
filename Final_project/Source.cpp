

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <map>


#include "shader.h"
#include "cube.h"
#include "pyramid.h"
#include "prism.h"
#include <iostream>
#include <string> 
#include <conio.h>
#include <vector>
#include "object3D.h"

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
// settings
unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 700;
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character*> Characters;
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


vector<Object3D*> objects;
int CHOSEN_INDEX = 0;


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
enum State{
	normal,
	change
};
enum CameraType {
	direct,
	reversed
};
CameraType cameraType = direct;
State STATE = normal;
bool culling = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
void clear_keyboard_buffer() {
	while (_kbhit()) _getch();
}

unsigned int VAO, VBO;
string get_status() {
	string stat = "CameraPos: " + to_string(cameraPos[0]) + "," + to_string(cameraPos[1]) + "," + to_string(cameraPos[2]);
	if (STATE == normal) {
		stat = stat + " | State: Normal";
	}
	else {
		stat = stat + " | State: On change";
	}
	stat += " | fps: " + to_string(1 / deltaTime);
	if (cameraType == direct) {
		stat = stat + " | cam-mouse-move: Not-reversed";
	}
	else {
		stat = stat + " | cam-mouse-move: Reversed";
	}
	if (culling ) {
		stat = stat + " | Culling: ON";
	}
	else {
		stat = stat + " | Culling: OFF";
	}
	return stat;
}
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// OpenGL state
	// ------------
	if (culling) {
		glEnable(GL_CULL_FACE);
		
	}
	//glFrontFace(GL_CW);
	//Also make sure to add a call to OpenGL to specify that triangles defined by a clockwise ordering
	//	are now 'front-facing' triangles so the cube is rendered as normal :
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// build and compile our shader zprogram
	// ------------------------------------

	Shader ourShader = Shader("camera.vs", "camera.fs");
	
	
	Shader textShader("text.vs", "text.fs");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, "arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//cout << "texture for text " << texture << endl;
		// now store character for later use
		Character* character = new Character;
		character->TextureID = texture;
		character->Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character->Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character->Advance = face->glyph->advance.x;
		Characters.insert(std::pair<char, Character*>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	Pyramid::setShapes();
	Cube::setShapes();
	Prism::setShapes();


	// load and create a texture 
	// -------------------------
	unsigned int texture1;

	// texture 1
	// ---------

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	//ourShader.setInt("texture1", 1);
//	textShader.setInt("texture1", 0);
	

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//for (unsigned int i = 0; i < objects.size(); i++)
		//{
			
			//glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1);
		//}
		// bind textures on corresponding texture units

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		// render boxes

		for (unsigned int i = 0; i < objects.size(); i++)
		{
			
			glBindVertexArray(((Cube*)objects.at(i))->getVAO());
			//cout<< ((Cube*)objects.at(i))->getVAO()<<endl;
			//cout << objects.at(i)->len << endl;
			glm::mat4 model = objects.at(i)->getModel(); // make sure to initialize matrix to identity matrix first

			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, objects.at(i)->len);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		RenderText(textShader, get_status(), 2.0f, 5.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		delete(objects.at(i));
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	clear_keyboard_buffer();



	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && STATE == normal) {
		glm::vec3 c_pos = cameraPos + 5.0f * cameraFront;
		Cube* cube = new Cube(c_pos, 0, 0, 0, 1.0f, 1.0f, 1.0f);
		objects.push_back(cube);
		CHOSEN_INDEX = objects.size() - 1;
		STATE = change;
		glfwWaitEventsTimeout(0.5);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && STATE == normal) {

		glm::vec3 c_pos = cameraPos + 5.0f * cameraFront;
		Pyramid* pyramid = new Pyramid(c_pos, 0, 0, 0, 1.0f, 1.0f, 1.0f);
		objects.push_back(pyramid);
		CHOSEN_INDEX = objects.size() - 1;
		STATE = change;
		glfwWaitEventsTimeout(0.5);
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && STATE == normal) {
		glm::vec3 c_pos = cameraPos + 5.0f * cameraFront;
		Prism* prism = new Prism(c_pos, 0, 0, 0, 1.0f, 1.0f, 1.0f);
		objects.push_back(prism);
		CHOSEN_INDEX = objects.size() - 1;
		STATE = change;
		glfwWaitEventsTimeout(0.5);
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;

	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->x_coef *= 1.3f;
		glfwWaitEventsTimeout(0.5);

	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->x_coef /= 1.3f;
		glfwWaitEventsTimeout(0.5);

	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->y_coef *= 1.3f;
		glfwWaitEventsTimeout(0.5);

	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->y_coef /= 1.3f;
		glfwWaitEventsTimeout(0.5);

	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->z_coef *= 1.3f;
		glfwWaitEventsTimeout(0.5);

	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->z_coef /= 1.3f;
		glfwWaitEventsTimeout(0.5);

	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->yaw += 1.0f;
		glfwWaitEventsTimeout(0.2);

	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->yaw -= 1.0f;
		glfwWaitEventsTimeout(0.2);

	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->pitch += 1.0f;
		glfwWaitEventsTimeout(0.2);

	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->pitch -= 1.0f;
		glfwWaitEventsTimeout(0.2);

	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->roll += 1.0f;
		glfwWaitEventsTimeout(0.2);

	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && STATE == change) {
		objects.at(CHOSEN_INDEX)->roll -= 1.0f;
		glfwWaitEventsTimeout(0.2);

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += glm::normalize(cameraUp) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= glm::normalize(cameraUp) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && STATE == change) {
		STATE = normal;
		glfwWaitEventsTimeout(2.5);
		cout << "Going Normal" << endl;
	}

	if (STATE == change) {
		objects.at(CHOSEN_INDEX)->position = cameraPos + 5.0f * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && STATE == normal) {
		long min_dis = LONG_MAX;
		long tresh = 5;
		int min_idx = -1;
		for (size_t i = 0; i < objects.size(); i++)
		{
			long dis = distance(cameraPos, objects.at(i)->position);
			if (dis < tresh && dis < min_dis) {
				min_dis = dis;
				min_idx = i;
			}
		}
		if (min_idx != -1) {
			STATE = change;
			CHOSEN_INDEX = min_idx;
			cout << CHOSEN_INDEX << " choosed for changing "<<min_dis << endl;
		}
		glfwWaitEventsTimeout(2.5);
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		if (cameraType == direct) {
			cameraType = reversed;
			cout << "camera reversed" << endl;
		}
		else {
			cameraType = direct;
			cout << "camera normal" << endl;
		}
		glfwWaitEventsTimeout(5);
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		if (culling ) {
			culling = false;
			cout << "culling off" << endl;
			glDisable(GL_CULL_FACE);
		}
		else {
			culling = true;
			cout << "culling on" << endl;
			glEnable(GL_CULL_FACE);
		}
		glfwWaitEventsTimeout(5);
	}
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	if (cameraType == reversed) {
		xoffset *= -1 ;
		yoffset *= -1;
	}
	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character* ch = (Character*)(Characters[*c]);

		float xpos = x + ch->Bearing.x * scale;
		float ypos = y - (ch->Size.y - ch->Bearing.y) * scale;

		float w = ch->Size.x * scale;
		float h = ch->Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch->TextureID);
		//cout << "TEXT Textur "<< ch->TextureID << endl;
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch->Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

