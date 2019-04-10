/* Josefine Klintberg, OpenGL template */

#include <iostream>

//Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//Include GLFW, define for being able to use glad
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

//Include header files
#include "Shader.hpp"

//Math
#include <cmath>

//Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

using namespace std;

int main() {

	float  time; 
	GLint  location_time, location_object;

	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW, make sure to use correct version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a window
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);

	int screenWidth, screenHeight;

	// Handles framebuffer resize callbacks
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// If we couldn't initialize the window
	if (nullptr == window){
		cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	//Set the window to be the current context
	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()){
		cout << "Failed to initialize GLEW" << endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	/* <---------------------- Compilation of Shaders ----------------------> */

	Shader myShader("vertex.glsl", "fragment.glsl");

	/* <---------------------- Vertex, index and transformation data ----------------------> */

	// Set up vertex data 
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};



	/* <---------------------- Vertex data and buffer objects ----------------------> */

	//Create vertex buffer object, vertex array object, element buffer object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards, don't unbind VAO unless it's not necessary
	// glBindVertexArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0); // The call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO


	location_object = glGetUniformLocation(myShader.ID, "stackTransf");

	location_time = glGetUniformLocation(myShader.ID, "time"); if (location_time == -1) { 
		cout  << "Unable  to  locate  variable'time'in  shader!" << endl;
	}


	// Rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// Check key press event
		glfwPollEvents();

		/* <--- Rendering loop ---> */

		//	Update time
		time = (float)glfwGetTime(); 
		glUseProgram(myShader.ID); //  Activate  the  shader  to set its  variables
		glUniform1f(location_time , time); // Copy  the  value to the  shader  program

		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Set clear color, background
		glClear(GL_COLOR_BUFFER_BIT);


		// Drawing
		myShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); //No need to do every time

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// De-allocate resources
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}
