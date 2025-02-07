#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <render/shader.h>
#include <render/texture.h>

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

static GLFWwindow *window;
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// OpenGL camera view parameters
static glm::vec3 eye_center(300, 300.0f, 0);
static glm::vec3 lookat(0, 0, 0);
static glm::vec3 up(0, 1, 0);

// View control 
static float viewAzimuth = 0.0f;
static float viewPolar = 0.0f;
static float viewDistance = 300.0f;
static bool rotating = false;

struct Building
{
	glm::vec3 position; // Position of the box
	glm::vec3 scale;	// Size of the box in each axis

	GLfloat vertex_buffer_data[72] = {
		// Vertex definition for a canonical box
		// Front face
		-1.0f,
		-1.0f,
		1.0f,
		1.0f,
		-1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		-1.0f,
		1.0f,
		1.0f,

		// Back face
		1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		-1.0f,
		1.0f,
		1.0f,
		-1.0f,

		// Left face
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		-1.0f,
		1.0f,
		1.0f,
		-1.0f,
		1.0f,
		-1.0f,

		// Right face
		1.0f,
		-1.0f,
		1.0f,
		1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		1.0f,
		-1.0f,
		1.0f,
		1.0f,
		1.0f,

		// Top face
		-1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		-1.0f,

		// Bottom face
		-1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		-1.0f,
		1.0f,
		-1.0f,
		-1.0f,
		1.0f,
	};

	GLfloat color_buffer_data[72] = {
		// Front, red
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,

		// Back, yellow
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,

		// Left, green
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,

		// Right, cyan
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,

		// Top, blue
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,

		// Bottom, magenta
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
	};

	GLuint index_buffer_data[36] = {
		// 12 triangle faces of a box
		0,
		1,
		2,
		0,
		2,
		3,

		4,
		5,
		6,
		4,
		6,
		7,

		8,
		9,
		10,
		8,
		10,
		11,

		12,
		13,
		14,
		12,
		14,
		15,

		16,
		17,
		18,
		16,
		18,
		19,

		20,
		21,
		22,
		20,
		22,
		23,
	};
	GLfloat uv_buffer_data[48] = {
		// Front
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		// Back
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		// Left
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		// Right
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		// Top
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		// Bottom
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	// OpenGL buffers
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint colorBufferID;
	GLuint uvBufferID;
	GLuint textureID;

	// Shader variable IDs
	GLuint mvpMatrixID;
	GLuint textureSamplerID;
	GLuint programID;

	void initialize(glm::vec3 position, glm::vec3 scale)
	{
		// Define scale of the building geometry
		this->position = position;
		this->scale = scale;

		// Create a vertex array object
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// Create a vertex buffer object to store the vertex data
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

		for (int i = 0; i < 72; ++i)
			color_buffer_data[i] = 1.0f;

		// Create a vertex buffer object to store the color data
		// TODO:
		glGenBuffers(1, &colorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

		// TODO: Create a vertex buffer object to store the UV data
		glGenBuffers(1, &uvBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);

		// Create an index buffer object to store the index data that defines triangle faces
		glGenBuffers(1, &indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

		// Create and compile our GLSL program from the shaders
		programID = LoadShaders("../lab2/box.vert", "../lab2/box.frag");
		if (programID == 0)
		{
			std::cerr << "Failed to load shaders." << std::endl;
		}

		// Get a handle for our "MVP" uniform
		mvpMatrixID = glGetUniformLocation(programID, "MVP");

		// TODO: Load a texture
		// Generate a random number between 1 and 4
		int textureNum = 1 + (rand() % 4);

		// Create the filename dynamically
		std::string textureFile = "../lab2/facade" + std::to_string(textureNum) + ".jpg";

		// Load the random texture
		textureID = LoadTexture(textureFile.c_str());

		// TODO: Get a handle to texture sampler
		textureSamplerID = glGetUniformLocation(programID, "textureSampler");
	}

	void render(glm::mat4 cameraMatrix)
	{
		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

		// TODO: Model transform
		// -----------------------
		glm::mat4 modelMatrix = glm::mat4();
		modelMatrix = glm::translate(modelMatrix, position);
		// Scale the box along each axis to make it look like a building
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		// -----------------------

		// Set model-view-projection matrix
		glm::mat4 mvp = cameraMatrix * modelMatrix;
		glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

		// TODO: Enable UV buffer and texture sampler
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(textureSamplerID, 0);

		// Draw the box
		glDrawElements(
			GL_TRIANGLES,	 // mode
			36,				 // number of indices
			GL_UNSIGNED_INT, // type
			(void *)0		 // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		// glDisableVertexAttribArray(2);
	}

	void cleanup()
	{
		glDeleteBuffers(1, &vertexBufferID);
		glDeleteBuffers(1, &colorBufferID);
		glDeleteBuffers(1, &indexBufferID);
		glDeleteVertexArrays(1, &vertexArrayID);
		// Enable these to clean up UV and texture buffers
		// glDeleteBuffers(1, &uvBufferID);
		// glDeleteTextures(1, &textureID);
		glDeleteProgram(programID);
	}
};

static float randomFloat() {
	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return r;
}
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	viewDistance -= yoffset * 10.0f; // Adjust zoom speed by modifying the multiplier
	if (viewDistance < 50.0f)
		viewDistance = 50.0f; // Prevent too much zoom-in
	if (viewDistance > 1000.0f)
		viewDistance = 1000.0f; // Prevent too much zoom-out

	eye_center.x = viewDistance * cos(viewAzimuth);
	eye_center.z = viewDistance * sin(viewAzimuth);
}
int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Lab 2", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to open a GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		std::cerr << "Failed to initialize OpenGL context." << std::endl;
		return -1;
	}

	srand(2024);

	// Background
	glClearColor(163 / 255.0f, 227 / 255.0f, 255 / 255.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// TODO: Create more buildings
    // ---------------------------
	std::vector<Building> buildings;
	for (int x = -4; x <= 4; ++x)
	{
		for (int z = -4; z <= 4; ++z)
		{
			Building b;
			float height = 10 + (rand() % 50); // Random height between 10-50
			b.initialize(glm::vec3(x * 32, 0, z * 32), glm::vec3(16, height, 16));
			buildings.push_back(b);
		}
	}
	Building b;
	b.initialize(glm::vec3(0, 0, 0), glm::vec3(30, 30, 30));
	buildings.push_back(b);
	Building tallest;
	tallest.initialize(glm::vec3(0, 0, 0), glm::vec3(20, 100, 20)); // Fixed tallest
	buildings.push_back(tallest);

	for (int x = -4; x <= 4; ++x)
	{
		for (int z = -4; z <= 4; ++z)
		{
			if (x == 0 && z == 0)
				continue; // Skip center (already placed)

			Building b;
			float height = 10 + (rand() % 50);
			b.initialize(glm::vec3(x * 32, 0, z * 32), glm::vec3(16, height, 16));
			buildings.push_back(b);
		}
	}

	// ---------------------------

	// TODO: Prepare a perspective camera
	// ------------------------------------
	// Prepare a perspective camera

	// glm::mat4 projectionMatrix;
	// glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
	glm::float32 FoV = 45;
	glm::float32 zNear = 0.1f;	  // Lower it to allow closer objects
	glm::float32 zFar = 10000.0f; // Increase it to allow more depth
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, zNear, zFar);

	// ------------------------------------

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Set camera view matrix
		// ------------------------------
		// Set camera view matrix

		// Note that we construct the view matrix in the loop to support view changes initiated by keyboard events. If your camera is fixed, you can move this function out of the loop.
		// ------------------------------
		// glm::mat4 viewMatrix;
		glm::mat4 viewMatrix = glm::lookAt(eye_center, lookat, up);
		// ------------------------------

		// Animation
		static double lastTime = glfwGetTime();
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;
		if (rotating) {
			viewAzimuth += 1.0f * deltaTime; 
			if (viewAzimuth > M_PI * 2) viewAzimuth -= M_PI * 2;
			eye_center.x = viewDistance * cos(viewAzimuth);
			eye_center.z = viewDistance * sin(viewAzimuth);
		}

		// For convenience, we multiply the projection and view matrix together and pass a single matrix for rendering
		glm::mat4 vp = projectionMatrix * viewMatrix;

		// Render the building
		for (int i = 0; i < buildings.size(); ++i)
			buildings[i].render(vp);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (!glfwWindowShouldClose(window));

	// Clean up
	for (int i = 0; i < buildings.size(); ++i)
		buildings[i].cleanup();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		std::cout << "Space key is pressed." << std::endl;
		rotating = !rotating;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		std::cout << "Reset." << std::endl;
	}

	if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		viewPolar -= 0.1f;
		eye_center.y = viewDistance * cos(viewPolar);
	}

	if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		viewPolar += 0.1f;
		eye_center.y = viewDistance * cos(viewPolar);
	}

	if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		viewAzimuth -= 0.1f;
		eye_center.x = viewDistance * cos(viewAzimuth);
		eye_center.z = viewDistance * sin(viewAzimuth);
	}

	if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		viewAzimuth += 0.1f;
		eye_center.x = viewDistance * cos(viewAzimuth);
		eye_center.z = viewDistance * sin(viewAzimuth);
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
