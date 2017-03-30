// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include<iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <lodepng.h>
using namespace glm;

#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "text2D.hpp"
#include "quaternion_utils.hpp"
#include "graphics_glfw.h"
#include "player.h"

#include "PxPhysics.h"
#include "PxScene.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "PxPhysicsAPI.h"
#include <pxphysicsapi.h>
#include <pxdefaulterrorcallback.h>
#include <pxdefaultallocator.h>


extern int entityMax;
GLFWwindow* window ;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	
	window = InitGraphics();
	InitEntitySystem(entityMax);
	initModelSystem();

	Player_S* p2 = newPlayer("aiai.obj","uvmap.DDS",glm::vec3(-5.00f, 5.0f, 15.0f),glm::vec3(1,1,1),glm::quat (-.14f,0.02f,-0.97f,-0.2f));
	Player_S* p1 = newPlayer("model.obj","uvmap.DDS",glm::vec3(0,0,-15),glm::vec3(3,3,3),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	Player_S* p3 = newPlayer("ballkirby.obj","uvmap.DDS",glm::vec3(0.00f, 7.0f, 20.0f),glm::vec3(.25f,.25f,.25f),glm::quat (0,0,0,0));

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);



	char * filename = ("aiai.png");
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

//decode PNG
  unsigned error = lodepng::decode(image, width, height, filename);
	
  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
  else{
  std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
  }
	
	// Initialize our little text library with the Holstein font
	initText2D( "Holstein.DDS" );

	// For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	int nbFrames = 0;
	

	do{

		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime); 
		lastFrameTime = currentTime;

		if ( currentTime - lastTime >= 1.0 ){ 

			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
//{ // Euler
// 
//			// As an example, rotate arount the vertical axis at 180°/sec
//			gOrientation1.y += 3.14159f/2.0f * deltaTime;
// 
//			// Build the model matrix
//			glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
//			glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
//			glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
//			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
// 
//			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
// 
//			// Send our transformation to the currently bound shader, 
//			// in the "MVP" uniform
//			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
//			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
//			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
// 
//			// Draw the triangles !
//			glDrawElements(
//				GL_TRIANGLES,      // mode
//				indices.size(),    // count
//				GL_UNSIGNED_SHORT,   // type
//				(void*)0           // element array buffer offset
//			);
// 
//		}

	entityDrawAll();

		char text[256];
		sprintf(text,"%.2f sec", glfwGetTime() );
		printText2D(text, 10, 500, 60);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
	
	glDeleteVertexArrays(1, &VertexArrayID);
	CloseEntitySystem();
	closeModelSystem();
	cleanupText2D();
	glfwTerminate();

	return 0;
}