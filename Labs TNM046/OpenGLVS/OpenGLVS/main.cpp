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
	GLint  location_time, location_object, location_light;
	GLfloat modelview, projection;
	MatrixStack myStack; //Stack used for transformations
	TriangleSoup myShape; 

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

	/* <---------------------- Compilation of Shader ----------------------> */

	Shader myShader("vertex.glsl", "fragment.glsl");

	/* <--------------- Vertex, index and transformation data ---------------> */
	
	//Is now done inside the triangle Soup class

	/* <---------------------- Create and send vertex data and buffer objects ----------------------> */

	//Is now done inside the triangle Soup class

	/* <---------------------- Shader variables ----------------------> */

	location_object = glGetUniformLocation(myShader.ID, "stackTransf");

	location_light = glGetUniformLocation(myShader.ID, "lightDirection");
	
	location_time = glGetUniformLocation(myShader.ID, "time"); if (location_time == -1) { 
		cout  << "Unable  to  locate  variable'time'in  shader!" << endl;
	}

	modelview = glGetUniformLocation(myShader.ID, "MV");

	projection = glGetUniformLocation(myShader.ID, "P");
	//glUniformMatrix4fv(projection, 1, GL_FALSE, projection.glLoadIdentity()); //Replace the current matrix with identity matrix

	//Note, we use the upper 3x3 of the MV matrix as normal matrix

	/* <---------------------- Create objects ----------------------> */

	// Intialize the matrix to an identity transformation
	myStack.init();

	//myShape.createSphere(0.8, 50);
	myShape.createBox(0.2, 0.2, 1);

	// Rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// Check key press event
		glfwPollEvents();

		/* <--- Rendering loop ---> */

		//	Update time
		time = (float)glfwGetTime(); 
		glUseProgram(myShader.ID); //  Activate the shader to set its variables
		glUniform1f(location_time , time); // Copy  the  value to the shader program

		GLuint loc = glGetUniformLocation(myShader.ID, "lightDirection");
		glUniform3fv(loc, sizeof(loc), (float*)loc);

		//// Clear the colorbuffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f); //Set clear color, background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_CULL_FACE); //Enable back face culling
		//glCullFace(GL_FRONT_AND_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Set to wireframe mode


		//// Drawing
		myShader.use();
		
		//glBindVertexArray(VAO);
		////glDrawArrays(GL_TRIANGLES, 0, 12*3);

		//glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);

		myShape.render();
		//glBindVertexArray(0); //No need to do every time
		
		//Start the stack operations, draw the scene with transformation
		myStack.push();

			// The view transformations, world frame
			//Rotate a small angle around x
			//myStack.rotX(M_PI / 12);
			//myStack.rotY(M_PI / 12);

			// Update the transformation matrix in the shader
			glUniformMatrix4fv(location_object, 1, GL_FALSE, myStack.getCurrentMatrix());

			//Add the object to the scene
			myStack.push();
			myStack.scale(0.5); //Scale to fit screen

			//myStack.rotY(M_PI*time / 12); //Orbit rotation
			//myStack.translate(1, 0, 0); //Move the object along the x-axis
			myStack.rotY(M_PI*time/ 12); //Rotate around own axis
			//myStack.rotZ(time*M_PI/20); //Rotate around own axis

			// Update the transformation matrix in the shader
			glUniformMatrix4fv(location_object, 1, GL_FALSE, myStack.getCurrentMatrix());
			
			//Restore the matrix
			myStack.pop();

			

		//Restore the initial matrix
		myStack.pop();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}
