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

//#include <PxPhysics.h>
//#include <PxScene.h>
//#include <PxRigidDynamic.h>
//#include <PxShape.h>
//#include <PxPhysicsAPI.h>
//#include <pxphysicsapi.h>
//#include <pxdefaulterrorcallback.h>
//#include <pxdefaultallocator.h>
//#include <PxTolerancesScale.h>
//#include <common/PxPhysXCommonConfig.h>
//#include <vehicle/PxVehicleSDK.h>

using namespace glm;
//using namespace physx;

#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "text2D.hpp"
#include "quaternion_utils.hpp"
#include "graphics_glfw.h"
#include "player.h"
#include "simple_logger.h"
#include "SDL_mixer.h"


extern int entityMax;

GLFWwindow* window;

//// Init Phy sx functions and parameters
//static class PxPhysics* gPhysicsSDK = NULL;
//static class PxDefaultErrorCallback gDefaultErrorCallback;
//static class PxDefaultAllocator gDefaultAllocatorCallback;
//
//void InitializePhysX() {
//	PxFoundation *mFoundation = NULL;
//
//	printf("creating Foundation\n");
// // create foundation object with default error and allocator callbacks.
// mFoundation = PxCreateFoundation(
// PX_PHYSICS_VERSION,
// gDefaultAllocatorCallback,
// gDefaultErrorCallback);
//
//   gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,  PxTolerancesScale() );
//   if(gPhysicsSDK == NULL) {
// slog("Error creating PhysX device.");
// slog("Exiting...");
// exit(1);
//   }
//}
//void ShutdownPhysX() {
//   gPhysicsSDK->release();
//}

//end Physx init func & param



int main( void )
{
//Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                   
                }

  Mix_Music *level_music;
  level_music = Mix_LoadMUS( "balrog.mid" );
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

	int mapNum = 1;

	Player_S* monkey = newPlayer("aiai.obj","Dirt03.bmp",glm::vec3(-5.00f, 5.0f, 15.0f),glm::vec3(1,1,1),glm::quat (-.14f,0.02f,-0.97f,-0.2f));
	Player_S* map = newPlayer("model3.obj","floor_tiles.bmp",glm::vec3(0,0,-15),glm::vec3(3,3,3),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	Player_S* ball = newPlayer("ballkirby.obj","ball1.bmp",glm::vec3(0.00f, 7.0f, 20.0f),glm::vec3(.25f,.25f,.25f),glm::quat (0,0,0,0));
	Player_S* ring = newPlayer("Ring.obj","blondhair.bmp",glm::vec3(0.00f, 6.0f, -20.0f),glm::vec3(1,1,1),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	
	Player_S* target1 = newPlayer("crate.obj","greenhair.bmp",glm::vec3(-15.00f, -25.0f, 5.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	Player_S* target2 = newPlayer("crate3.obj","redhair.bmp",glm::vec3(0.00f, -25.0f,-5.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	Player_S* target3 = newPlayer("crate2.obj","blondhair.bmp",glm::vec3(15.00f, -25.0f, 5.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	
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

  
	
	// Initialize our little text library with the Holstein font
	initText2D( "Holstein.DDS" );

	// For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	int nbFrames = 0;
	

	do{
		if (glfwGetKey( window,GLFW_KEY_ENTER ) == GLFW_PRESS)
			{
				 //mapNum++;
				ChangeMap(map,mapNum);
			    slog("changed the map");
			}
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime); 
		lastFrameTime = currentTime;

		  if( Mix_PlayingMusic() == 0 )
                    {
                        //Play the music
                        if( Mix_PlayMusic( level_music, -1 ) == -1 )
                        {
                            return 1;
                        } 
					}

		if ( currentTime - lastTime >= 1.0 ){ 

			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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