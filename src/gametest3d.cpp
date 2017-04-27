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


//#define BTQUAT2GLMQUAT(GLM,BT) (GLM).x = (BT).x(); \
//							(GLM).y = (BT).y(); \
//							(GLM).z = (BT).z(); \
//							(GLM).w = (BT).w(); 


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
	Player_S monkey;
	btRigidBody *rigidBody3;
	btRigidBody *rigidBody4;
	btRigidBody *rigidBody5;
	btRigidBody *rigidBody6;
	btRigidBody *rigidBody7;
	btVector3 t1;
	btVector3 t2;
	btVector3 t3;
	int points = 0;
	int touch = 0;
	extern int mapNum;


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
				slog("Pressed Enter");
		
				}
	   }

	   if (key == GLFW_KEY_SPACE)
	   {
		   if (action == GLFW_RELEASE)
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

	mapNum = 1;

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
	glm::vec3 v0;


	btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	btCollisionShape* boxCollisionShapePlatform = new btBoxShape(btVector3(30.0f, 1.0f, 12.0f));
	btCollisionShape* boxCollisionShapeTarget = new btBoxShape(btVector3(8.4, 1.f, 10.4));
	btCollisionShape* ballCollisionShape = new btSphereShape(1);
	

	t1 = btVector3(-36, -49, -55);
	t2 = btVector3(-1, -49, -60);
	t3 = btVector3(34, -49, -65);
	btTransform spawn = btTransform( btQuaternion(0, 0, 0, 1), btVector3(0, 1, 10) );

	btDefaultMotionState* motionstatemonkey = new btDefaultMotionState(btTransform(
		btQuaternion(0, 0, 0, 1),
		btVector3(0, 1, 10)));
	
	btDefaultMotionState* motionstateRing1 = new btDefaultMotionState(btTransform(
		btQuaternion(0.71f, 0.00f, -0.71f, 0.00f),
		btVector3(-5, -7, -20)
	));
	
	btDefaultMotionState* motionstateRing2 = new btDefaultMotionState(btTransform(
		btQuaternion(0.71f, 0.00f, -0.71f, 0.00f),
		btVector3(5,-7,-20)
	));

	btDefaultMotionState* motionstateTarget1 = new btDefaultMotionState(btTransform(
		btQuaternion(0.71f, 0.00f, -0.71f, 0.00f),
		btVector3(t1)
	));


	btDefaultMotionState* motionstateTarget2 = new btDefaultMotionState(btTransform(
		btQuaternion(0.71f, 0.00f, -0.71f, 0.00f),
		btVector3(t2)
	));


	btDefaultMotionState* motionstateTarget3 = new btDefaultMotionState(btTransform(
		btQuaternion(0.71f, 0.00f, -0.71f, 0.00f),
		btVector3(t3)
	));

	btDefaultMotionState* motionstateMap = new btDefaultMotionState(btTransform(
		btQuaternion( -.06, -.71, .08, .71),
		btVector3(0, -8, -15)
	));


	btRigidBody::btRigidBodyConstructionInfo rigidBodyMON(
		1,                  // mass, in kg. 0 -> Static object, will never move.
		motionstatemonkey,
		ballCollisionShape,  // collision shape of body
		btVector3(2, 0,22)    // local inertia //center of mass aka moment of interia
	);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyMap(
		.0,                  // mass, in kg. 0 -> Static object, will never move.
		motionstateMap,
		boxCollisionShapePlatform,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyTarget1(
		.0,                  // mass, in kg. 0 -> Static object, will never move.
		motionstateTarget1,
		boxCollisionShapeTarget,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);


	btRigidBody::btRigidBodyConstructionInfo rigidBodyTarget2(
		.0,                  // mass, in kg. 0 -> Static object, will never move.
		motionstateTarget2,
		boxCollisionShapeTarget,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);


	btRigidBody::btRigidBodyConstructionInfo rigidBodyTarget3(
		.0,                  // mass, in kg. 0 -> Static object, will never move.
		motionstateTarget3,
		boxCollisionShapeTarget,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyRing1(
		.0,                  // mass, in kg. 0 -> Static object, will never move.
		motionstateRing1,
		boxCollisionShape,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyRing2(
		.0,                  // mass, in kg. 0 -> Static object, will never move.
		motionstateRing2,
		boxCollisionShape,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);
	
	btRigidBody *rigidBody = new btRigidBody(rigidBodyMON);
	rigidbodies.push_back(rigidBody);
	dynamicsWorld->addRigidBody(rigidBody);

	btRigidBody *rigidBody2 = new btRigidBody(rigidBodyMap);
	rigidbodies.push_back(rigidBody2);
	dynamicsWorld->addRigidBody(rigidBody2);

	rigidBody3 = new btRigidBody(rigidBodyTarget1);
	rigidbodies.push_back(rigidBody3);
	dynamicsWorld->addRigidBody(rigidBody3);

	 rigidBody4 = new btRigidBody(rigidBodyTarget2);
	rigidbodies.push_back(rigidBody4);
	dynamicsWorld->addRigidBody(rigidBody4);

	rigidBody5 = new btRigidBody(rigidBodyTarget3);
	rigidbodies.push_back(rigidBody5);
	dynamicsWorld->addRigidBody(rigidBody5);

	rigidBody6 = new btRigidBody(rigidBodyRing1);
	rigidbodies.push_back(rigidBody6);
	dynamicsWorld->addRigidBody(rigidBody6);

	btRigidBody *rigidBody7 = new btRigidBody(rigidBodyRing2);
	rigidbodies.push_back(rigidBody7);
	dynamicsWorld->addRigidBody(rigidBody7);

	monkey = *newPlayer("ballobj.obj","ball1.bmp",glm::vec3(0.00f, -0.5f, 14.0f),glm::vec3(.23f,.23f,.23f),glm::quat (0,0,0,0));
	map  =	 *newPlayer("model3.obj","floor_tiles.bmp",glm::vec3(0,-8,-15),glm::vec3(3,3,3),glm::quat (.71,-.06,-.71,.08));

	ring1 =  *newPlayer("Ring.obj","blondhair.bmp",glm::vec3(5.00f, -7.0f, -20.0f),glm::vec3(.5f,.5f,.5f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	ring2 =  *newPlayer("Ring2.obj","blondhair.bmp",glm::vec3(-5.00f, -7.0f, -20.0f),glm::vec3(.5f,.5f,.5f),glm::quat (0.71f,0.00f,-0.71f,0.00f));

	target1 = *newPlayer("crate.obj","greenhair.bmp",glm::vec3(-35.00f, -50.0f, -55.0f),glm::vec3(.9f, .1f, 1.1f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	target2 = *newPlayer("crate2.obj","redhair.bmp",glm::vec3(0.00f, -50.0f,-60.0f),glm::vec3(.9f,.1f,1.1f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	target3 = *newPlayer("crate3.obj","blondhair.bmp",glm::vec3(35.00f, -50.0f, -65.0f),glm::vec3(.9f, .1f, 1.1f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
	

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	

	initText2D( "Holstein.DDS" );

	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	int nbFrames = 0;

	do{

		btQuaternion O0 = rigidBody->getOrientation();

		btVector3 p0 = rigidBody->getCenterOfMassPosition();
		if (p0.y() <= -75)
		{
			ChangeMap();
			rigidBody->setWorldTransform(spawn);
			p0 = (btVector3(0, 1, 10));
		}
		
		BTVEC32GLMVEC3(monkey.Ent->model->position, p0);

		BTQUAT2GLMQUAT(monkey.Ent->model->orientation,O0);
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


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();
		entityDrawAll();
		
		char text1[256];
		char text2[256];
	
		sprintf(text1,"Level: %i", mapNum );
		sprintf(text2,"%i", monkey.points);

		if (monkey.points == 100)
		{
			dynamicsWorld->removeRigidBody(rigidBody7);
		}
		if (monkey.points == 200)
		{
			dynamicsWorld->removeRigidBody(rigidBody6);
		}
		printText2D(text1, 500, 500, 30);
		printText2D(text2, 600, 1040, 30);

		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		mydebugdrawer.SetMatrices(ViewMatrix, ProjectionMatrix);
		dynamicsWorld->debugDrawWorld();
		dynamicsWorld->stepSimulation(deltaTime, 1);
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
