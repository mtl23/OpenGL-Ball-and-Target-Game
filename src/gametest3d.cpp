// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include<iostream>
//#include<btBulletCollisionCommon.h>
//#include<btBulletDynamicsCommon.h>
// Include GLEW
#include <glew.h>

// Include GLFW
#include <glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
// Include GLM
#include <SDL_mixer.h>
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

#include "Physics.h"


	extern int entityMax;
	GLFWwindow* window;
	GLuint programID;
	Player_S ring1;
	Player_S ring2;
	btDiscreteDynamicsWorld* dynamicsWorld;
	Player_S target1;
	Player_S target2;
	Player_S target3;

	Player_S map;
	//Player_S ball;
	Player_S monkey;

	int points = 0;
	int mapnum = 1;



	class BulletDebugDrawer_DeprecatedOpenGL : public btIDebugDraw {
	public:
		void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix) {
			glUseProgram(0); // Use Fixed-function pipeline (no shaders)
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&pViewMatrix[0][0]);
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&pProjectionMatrix[0][0]);
		}
		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
			glColor3f(color.x(), color.y(), color.z());
			glBegin(GL_LINES);
			glVertex3f(from.x(), from.y(), from.z());
			glVertex3f(to.x(), to.y(), to.z());
			glEnd();
		}
		virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}
		virtual void reportErrorWarning(const char *) {}
		virtual void draw3dText(const btVector3 &, const char *) {}
		virtual void setDebugMode(int p) {
			m = p;
		}
		int getDebugMode(void) const { return 3; }
		int m;
	};

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
	int numboxes;
	numboxes = 10;
	//Generate positions & rotations for 100 monkeys
	std::vector<glm::vec3> positions(numboxes);
	std::vector<glm::quat> orientations(numboxes);
	for (int i = 0; i<numboxes; i++) {
		positions[i] = glm::vec3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10);
		orientations[i] = glm::normalize(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));

	}
	//Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	//Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//The world.
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));


	BulletDebugDrawer_DeprecatedOpenGL mydebugdrawer;
	dynamicsWorld->setDebugDrawer(&mydebugdrawer);

	std::vector<btRigidBody*> rigidbodies;

	// In this example, all monkeys will use the same collision shape : 
	// A box of 2m*2m*2m (1.0 is the half-extent !)
	btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < numboxes; i++) {

		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
			btQuaternion(orientations[i].x, orientations[i].y, orientations[i].z, orientations[i].w),
			btVector3(positions[i].x, positions[i].y, positions[i].z)
		));

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			.0,                  // mass, in kg. 0 -> Static object, will never move.
			motionstate,
			boxCollisionShape,  // collision shape of body
			btVector3(0, 0, 0)    // local inertia
		);
		btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);

		rigidbodies.push_back(rigidBody);
		dynamicsWorld->addRigidBody(rigidBody);

		// Small hack : store the mesh's index "i" in Bullet's User Pointer.
		//Will be used to know which object is picked. 
		//A real program would probably pass a "MyGameObjectPointer" instead.
		rigidBody->setUserPointer((void*)0);

	}

	monkey = *newPlayer("ballkirby.obj","ball1.bmp",glm::vec3(0.00f, -0.5f, 14.0f),glm::vec3(.25f,.25f,.25f),glm::quat (0,0,0,0));
	map  =	 *newPlayer("model3.obj","floor_tiles.bmp",glm::vec3(0,0,-15),glm::vec3(3,3,3),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	//ball =   *newPlayer("ballkirby.obj","ball1.bmp",glm::vec3(0.00f, 0.0f, 15.0f),glm::vec3(.25f,.25f,.25f),glm::quat (0,0,0,0));
	 
	ring1 =  *newPlayer("Ring.obj","blondhair.bmp",glm::vec3(5.00f, 2.0f, -20.0f),glm::vec3(.5f,.5f,.5f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	ring2 =  *newPlayer("Ring2.obj","blondhair.bmp",glm::vec3(-5.00f, 2.0f, -20.0f),glm::vec3(.5f,.5f,.5f),glm::quat (0.71f,0.00f,-0.71f,0.00f));

	target1 = *newPlayer("crate.obj","greenhair.bmp",glm::vec3(-15.00f, -50.0f, -45.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	target2 = *newPlayer("crate2.obj","redhair.bmp",glm::vec3(0.00f, -50.0f,-60.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	target3 = *newPlayer("crate3.obj","blondhair.bmp",glm::vec3(15.00f, -50.0f, -65.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	
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
	
		programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardTransparentShading.fragmentshader");
		glUseProgram(programID);
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
		dynamicsWorld->stepSimulation(deltaTime, 1);
		computeMatricesFromInputs();
		entityDrawAll();
		
		char text1[256];
		char text2[256];
	
		sprintf(text1,"Level: %i", mapnum );
		sprintf(text2,"%i", monkey.points);

		printText2D(text1, 500, 500, 30);
		printText2D(text2, 600, 1040, 30);

		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		mydebugdrawer.SetMatrices(ViewMatrix, ProjectionMatrix);
		dynamicsWorld->debugDrawWorld();

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
