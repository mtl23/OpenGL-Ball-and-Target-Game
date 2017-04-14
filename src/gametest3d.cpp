// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include<iostream>

// Include GLEW
#include <glew.h>

// Include GLFW
#include <glfw3.h>
//#include "btBulletDynamicsCommon.h"

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
#include "simple_logger.h"
#include "SDL_mixer.h"


	extern int entityMax;
	GLFWwindow* window;
		
	Player_S ring1;
	Player_S ring2;
	
	Player_S target1;
	Player_S target2;
	Player_S target3;

	Player_S map;
	Player_S ball;
	Player_S monkey;

	int points = 0;
	int mapnum = 1;

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	   if(key == GLFW_KEY_ENTER  )
	   {
			if( action ==GLFW_RELEASE)
				{
		
					ChangeMap();
				mapnum += 1;
				if(mapnum>3)
					  {
					  mapnum = 1;
					  }
				}
	   }

	   if(key == GLFW_KEY_SPACE)
	   {
			if( action ==GLFW_RELEASE)
				{
			   Pickup(&monkey);
				}
	   }
        
}


int main( void )
{

//	Initialize SDL_mixer


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

	glfwSetKeyCallback(window,key_callback);

	InitEntitySystem(entityMax);
	initModelSystem();

	monkey = *newPlayer("aiai.obj","Dirt03.bmp",glm::vec3(-5.00f, 5.0f, 15.0f),glm::vec3(1,1,1),glm::quat (-.14f,0.02f,-0.97f,-0.2f));
	map  =	 *newPlayer("model3.obj","floor_tiles.bmp",glm::vec3(0,0,-15),glm::vec3(3,3,3),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	ball =   *newPlayer("ballkirby.obj","ball1.bmp",glm::vec3(0.00f, 7.0f, 20.0f),glm::vec3(.25f,.25f,.25f),glm::quat (0,0,0,0));
	 
	ring1 =  *newPlayer("Ring.obj","redhair.bmp",glm::vec3(5.00f, 7.0f, -20.0f),glm::vec3(.71,.71,.71),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	ring2 =  *newPlayer("Ring2.obj","blondhair.bmp",glm::vec3(-5.00f, 7.0f, -20.0f),glm::vec3(.71,.71,.71),glm::quat (0.71f,0.00f,-0.71f,0.00f));

	target1 = *newPlayer("crate.obj","greenhair.bmp",glm::vec3(-15.00f, -50.0f, -15.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	target2 = *newPlayer("crate2.obj","redhair.bmp",glm::vec3(0.00f, -50.0f,-20.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	target3 = *newPlayer("crate3.obj","blondhair.bmp",glm::vec3(15.00f, -50.0f, -25.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	
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

		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime); 
		lastFrameTime = currentTime;

		  if( Mix_PlayingMusic() == 0 )
                    {
                        ////Play the music
                        //if( Mix_PlayMusic( level_music, -1 ) == -1 )
                        //{
                        //    return 1;
                        //} 
					}

		if ( currentTime - lastTime >= 1.0 ){ 

			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		entityDrawAll();
		
		char text1[256];
		char text2[256];
	
		sprintf(text1,"Level: %i", mapnum );
		sprintf(text2,"%i", monkey.points);

		printText2D(text1, 500, 500, 30);
		printText2D(text2, 600, 1040, 30);

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
