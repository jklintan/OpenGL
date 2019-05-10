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
#include "MatrixStack.hpp"
#include "Utilities.hpp"
#include "TriangleSoup.hpp"
#include "Texture.hpp"

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif

//Math
#include <cmath>

//Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

using namespace std;

int main() {

	//Declaration of variables
	float  time; 
	GLint  location_time, location_object, location_light, location_view;
	GLfloat modelview, projection;
	MatrixStack myStack; //Stack used for transformations
	TriangleSoup Box, Sphere; 
	float PM[16] = { 0 }; //Initialize projection matrix
	Texture Text1, Text2;
	GLint location_tex;

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

	Shader lightningShader("vertex.glsl", "fragment.glsl");
	//Shader textureShader("vertex.glsl", "fragmentTex.glsl");

	/* <--------------- Vertex, index and transformation data ---------------> */
	
	//Is now done inside the triangle Soup class

	/* <---------------------- Create and send vertex data and buffer objects ----------------------> */

	//Is now done inside the triangle Soup class for objects

	/* <---------------------- Shader variables ----------------------> */

	location_object = glGetUniformLocation(lightningShader.ID, "model");

	location_view = glGetUniformLocation(lightningShader.ID, "view");

	location_light = glGetUniformLocation(lightningShader.ID, "lightDirection");
	
	location_time = glGetUniformLocation(lightningShader.ID, "time"); if (location_time == -1) { 
		cout  << "Unable  to  locate  variable 'time' in  shader!" << endl;
	}

	projection = glGetUniformLocation(lightningShader.ID, "projection");
	//glUniformMatrix4fv(projection, 1, GL_FALSE, projection.glLoadIdentity()); //Replace the current matrix with identity matrix

	//Note, we use the upper 3x3 of the MV matrix as normal matrix

	/* <---------------------- Texture handling ----------------------> */

	// Locate the sampler2D uniform in the shader program
	location_tex = glGetUniformLocation(lightningShader.ID, "ourTexture");
	// Generate one texture object with data from a TGA file
	Text1.createTexture("textures/box.tga");
	Text2.createTexture("textures/crate.tga");

	/* <---------------------- Create objects ----------------------> */

	// Intialize the matrix to an identity transformation
	myStack.init();

	//Sphere.createSphere(0.8, 50);
	Box.createBox(0.5, 0.5, 0.5);

	// Rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// Check key press event
		glfwPollEvents();

		/* <--- Rendering loop ---> */

		//	Update time
		time = (float)glfwGetTime(); 
		glUseProgram(lightningShader.ID); //  Activate the shader to set its variables
		glUniform1f(location_time , time); // Copy  the  value to the shader program

		//GLuint loc = glGetUniformLocation(lightningShader.ID, "lightDirection");
		//glUniform3fv(loc, sizeof(loc), (float*)loc);

		//// Clear the colorbuffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Set clear color, background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_CULL_FACE); //Enable back face culling
		//glCullFace(GL_FRONT_AND_BACK);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Set to wireframe mode

		//Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Text1.textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Text2.textureID);

		//// Drawing
		lightningShader.use();
		Box.render();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//Start the stack operations, draw the scene with transformation
		myStack.push();

			// The view transformations, world frame
			myStack.rotX(M_PI / 20);
			myStack.translate(0, -0.1, 0);
			//myStack.rotY(M_PI / 12);
			//myStack.rotY(M_PI);

			// Update the view matrix in the shader
			glUniformMatrix4fv(location_view, 1, GL_FALSE, myStack.getCurrentMatrix());

			//Add the object to the scene
			myStack.push();
			myStack.scale(0.8); //Scale to fit screen

			//myStack.rotY(M_PI*time / 12); //Orbit rotation
			//myStack.translate(1, 0, 0); //Move the object along the x-axis
			//myStack.rotY(time/ 10); //Rotate around own axis
			//myStack.rotZ(time*M_PI/20); //Rotate around own axis
			myStack.rotY(M_PI / 4);

			// Update the transformation matrix in the shader
			glUniformMatrix4fv(location_object, 1, GL_FALSE, myStack.getCurrentMatrix());

			//Restore the matrix
			myStack.pop();

			//Perform the perspective projection
			myStack.translate(0, 0, -2);
			myStack.mat4perspective(PM, M_PI/4, 1, 0.5, 10); //Turns PM into the correct projection matrix and multiply to current matrix
			glUniformMatrix4fv(projection, 1, GL_FALSE, PM); //Send the projection matrix to the vertex shader

		//Restore the initial matrix
		myStack.pop();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}