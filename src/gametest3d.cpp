// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

//GLFWwindow* window;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <AntTweakBar.h>

#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "text2D.hpp"
#include "quaternion_utils.hpp"
#include "entity.h"
#include "graphics_glfw.h"

extern int entityMax;

vec3 gPosition1(-5.00f, 5.0f, 15.0f); //the monkey
vec3 gOrientation1;
 
vec3 gPosition2( 0.0f, 0.0f, -15.0f); // the stage
quat gOrientation2 = quat(0.71f,0.00f,-0.71f,0.00f);

vec3 gPosition3( 0.0f, 7.0f, 20.0f); // the ball
quat gOrientation3;
 
bool gLookAtOther = true;
// The ARB_debug_output extension, which is used in this tutorial as an example,
// can call a function of ours with error messages.
// This function must have this precise prototype ( parameters and return value )
// See http://www.opengl.org/registry/specs/ARB/debug_output.txt , "New Types" : 
//	The callback function that applications can define, and
//	is accepted by DebugMessageCallbackARB, is defined as:
//	
//	    typedef void (APIENTRY *DEBUGPROCARB)(enum source,
//	                                          enum type,
//	                                          uint id,
//	                                          enum severity,
//	                                          sizei length,
//	                                          const char* message,
//	                                          void* userParam);
void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){

	printf("OpenGL Debug Output message : ");

	if(source == GL_DEBUG_SOURCE_API_ARB)					printf("Source : API; ");
	else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)	printf("Source : WINDOW_SYSTEM; ");
	else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)	printf("Source : SHADER_COMPILER; ");
	else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)		printf("Source : THIRD_PARTY; ");
	else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)		printf("Source : APPLICATION; ");
	else if(source == GL_DEBUG_SOURCE_OTHER_ARB)			printf("Source : OTHER; ");

	if(type == GL_DEBUG_TYPE_ERROR_ARB)						printf("Type : ERROR; ");
	else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)	printf("Type : DEPRECATED_BEHAVIOR; ");
	else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)	printf("Type : UNDEFINED_BEHAVIOR; ");
	else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)			printf("Type : PORTABILITY; ");
	else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)			printf("Type : PERFORMANCE; ");
	else if(type == GL_DEBUG_TYPE_OTHER_ARB)				printf("Type : OTHER; ");

	if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)				printf("Severity : HIGH; ");
	else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)		printf("Severity : MEDIUM; ");
	else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)			printf("Severity : LOW; ");

	// You can set a breakpoint here ! Your debugger will stop the program,
	// and the callstack will immediately show you the offending call.
	printf("Message : %s\n", message);
}


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	GLFWwindow* window = InitGraphics();
	InitEntitySystem(entityMax);

	//glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1); 

	//// Open a window and create its OpenGL context
	//window = glfwCreateWindow( 1024, 768, "3DGameMIDTERM", NULL, NULL);
	//if( window == NULL ){
	//	fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	//	getchar();
	//	glfwTerminate();
	//	return -1;
	//}
	//glfwMakeContextCurrent(window);

	//// Initialize GLEW
	//glewExperimental = true; // Needed for core profile
	//if (glewInit() != GLEW_OK) {
	//	fprintf(stderr, "Failed to initialize GLEW\n");
	//	getchar();
	//	glfwTerminate();
	//	return -1;
	//}

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(1024, 768);
	TwBar * EulerGUI = TwNewBar("Euler settings");
	TwBar * QuaternionGUI = TwNewBar("Quaternion settings");
	TwSetParam(EulerGUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(QuaternionGUI, NULL, "position", TW_PARAM_CSTRING, 1, "808 16");

	TwAddVarRW(EulerGUI, "Euler X", TW_TYPE_FLOAT, &gOrientation1.x, "step=0.01");
	TwAddVarRW(EulerGUI, "Euler Y", TW_TYPE_FLOAT, &gOrientation1.y, "step=0.01");
	TwAddVarRW(EulerGUI, "Euler Z", TW_TYPE_FLOAT, &gOrientation1.z, "step=0.01");
	TwAddVarRW(EulerGUI, "Pos X"  , TW_TYPE_FLOAT, &gPosition1.x, "step=0.1");
	TwAddVarRW(EulerGUI, "Pos Y"  , TW_TYPE_FLOAT, &gPosition1.y, "step=0.1");
	TwAddVarRW(EulerGUI, "Pos Z"  , TW_TYPE_FLOAT, &gPosition1.z, "step=0.1");
 
	TwAddVarRW(QuaternionGUI, "Quaternion", TW_TYPE_QUAT4F, &gOrientation2, "showval=true open=true ");
	TwAddVarRW(QuaternionGUI, "Use LookAt", TW_TYPE_BOOL8 , &gLookAtOther, "help='Look at the other monkey ?'");
 
	// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	//glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
	//glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
	//glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	//glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
	//glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar


	// Example 1 :
	//if ( GLEW_AMD_seamless_cubemap_per_texture ){
	//	printf("The GL_AMD_seamless_cubemap_per_texture is present, (but we're not goint to use it)\n");
	//	// Now it's legal to call glTexParameterf with the TEXTURE_CUBE_MAP_SEAMLESS_ARB parameter
	//	// You HAVE to test this, because obviously, this code would fail on non-AMD hardware.
	//}

	// Example 2 :
	if ( GLEW_ARB_debug_output ){
		printf("The OpenGL implementation provides debug output. Let's use it !\n");
		glDebugMessageCallbackARB(&DebugOutputCallback, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB); 
	}else{
		printf("ARB_debug_output unavailable. You have to use glGetError() and/or gDebugger to catch mistakes.\n");
	}


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/StandardShading.vertexshader", "shaders/StandardTransparentShading.fragmentshader" );
	//GLuint programIDTrans = LoadShaders( "shaders/StandardShading.vertexshader", "shaders/StandardTransparentShading.fragmentshader" );
	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("uvmap.DDS");
	GLuint Texture2 = loadBMP_custom("uvtemplate.bmp");
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	//GLuint TextureIDTrans  = glGetUniformLocation(programID, "myTextureSamplerTrans");

//////////////////monkey setups
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("aiai.obj", vertices, uvs, normals);
	
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

/////////// end monkey setups

//////////////////////////// the stage setups

	
	// Read our .obj file
	std::vector<glm::vec3> vertices2;
	std::vector<glm::vec2> uvs2;
	std::vector<glm::vec3> normals2;
	bool res2 = loadOBJ("model.obj", vertices2, uvs2, normals2);


	std::vector<unsigned short> indices2;
	std::vector<glm::vec3> indexed_vertices2;
	std::vector<glm::vec2> indexed_uvs2;
	std::vector<glm::vec3> indexed_normals2;
	indexVBO(vertices2, uvs2, normals, indices2, indexed_vertices2, indexed_uvs2, indexed_normals2);

	// Load it into a VBO

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices2.size() * sizeof(glm::vec3), &indexed_vertices2[0], GL_STATIC_DRAW);

	GLuint uvbuffer2;
	glGenBuffers(1, &uvbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs2.size() * sizeof(glm::vec2), &indexed_uvs2[0], GL_STATIC_DRAW);

	GLuint normalbuffer2;
	glGenBuffers(1, &normalbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer2);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals2.size() * sizeof(glm::vec3), &indexed_normals2[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint elementbuffer2;
	glGenBuffers(1, &elementbuffer2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices2.size() * sizeof(unsigned short), &indices2[0] , GL_STATIC_DRAW);


	///////////////end stage setups
	
	//////////////////////////// the ball setups

	
	// Read our .obj file
	std::vector<glm::vec3> vertices3;
	std::vector<glm::vec2> uvs3;
	std::vector<glm::vec3> normals3;

	std::vector<unsigned short> indices3;
	std::vector<glm::vec3> indexed_vertices3;
	std::vector<glm::vec2> indexed_uvs3;
	std::vector<glm::vec3> indexed_normals3;

	bool res3 = loadOBJ("ballkirby.obj", vertices3, uvs3, normals3);
	indexVBO(vertices3, uvs3, normals, indices3, indexed_vertices3, indexed_uvs3, indexed_normals3);

	// Load it into a VBO

	GLuint vertexbuffer3;
	glGenBuffers(1, &vertexbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices3.size() * sizeof(glm::vec3), &indexed_vertices3[0], GL_STATIC_DRAW);

	GLuint uvbuffer3;
	glGenBuffers(1, &uvbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs3.size() * sizeof(glm::vec2), &indexed_uvs3[0], GL_STATIC_DRAW);

	GLuint normalbuffer3;
	glGenBuffers(1, &normalbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer3);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals3.size() * sizeof(glm::vec3), &indexed_normals3[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint elementbuffer3;
	glGenBuffers(1, &elementbuffer3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices3.size() * sizeof(unsigned short), &indices3[0] , GL_STATIC_DRAW);
///////////

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	
	// Initialize our little text library with the Holstein font
	initText2D( "Holstein.DDS" );

	// For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	int nbFrames = 0;
	
	// Enable blending
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	do{

		// Measure speed
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime); 
		lastFrameTime = currentTime;

		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			// printf and reset
			//printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs();
		//glm::mat4 ProjectionMatrix = getProjectionMatrix();
		//glm::mat4 ViewMatrix = getViewMatrix();

			
		////// Start of the rendering of the first object //////
		
		// Use our shader
		glUseProgram(programID);
	
		//glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "programID"
		
			glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
			glm::mat4 ViewMatrix = glm::lookAt(
			glm::vec3( 0, 20, 37 ), // Camera is here
			glm::vec3( 0, 0, 0 ), // and looks here
			glm::vec3( 0, 1, 0 )  // Head is up (set to 0,-1,0 to look upside-down)
		);

		//// Send our transformation to the currently bound shader, 
		//// in the "MVP" uniform wth INPUTS
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
		//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix1[0][0]);


		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glm::vec3 lightPos = glm::vec3(0,-16,-24);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		

{ // Euler
 
			// As an example, rotate arount the vertical axis at 180°/sec
			gOrientation1.y += 3.14159f/2.0f * deltaTime;
 
			// Build the model matrix
			glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
			glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
			glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
 
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
 
			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
 
 
 
			// Draw the triangles !
			glDrawElements(
				GL_TRIANGLES,      // mode
				indices.size(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
 
		}

////// End of rendering of the first object //////

////// Start of the rendering of the second object //////


// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer2);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer2);

		glm::vec3 lightPos2 = glm::vec3(-16,-8,-24);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
{ // Quaternion
 
			// It the box is checked...
		//	if (gLookAtOther){
				// desiredDir = gPosition1-gPosition2;
				//vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y
 
				// Compute the desired orientation
			//	quat targetOrientation = normalize(LookAt(desiredDir, desiredUp));
 
				// And interpolate
				//gOrientation2 = RotateTowards(gOrientation2, targetOrientation, 1.0f*deltaTime);
	//		}
			//gOrientation2 = quat(0.71f,0.00f,-0.71f,0.00f); // (S,X,Y,Z)// change me with input
			glm::mat4 RotationMatrix = mat4_cast(gOrientation2);
			glm::mat4 TranslationMatrix = translate(mat4(), gPosition2); // A bit to the right
			glm::mat4 ScalingMatrix = scale(mat4(), vec3(3.0f, 3.0f, 3.0f));
			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
 
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
 
			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
 
 
			// Draw the triangles !
			glDrawElements(
				GL_TRIANGLES,      // mode
				indices2.size(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}

		////// End of rendering of the second object //////

//// Start of rendering of the third object //////

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer3);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer3);

		glm::vec3 lightPos3 = glm::vec3(16,-8,-24);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glm::mat4 RotationMatrix = mat4_cast(gOrientation3);
		glm::mat4 TranslationMatrix = translate(mat4(), gPosition3); // places into position
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(.25f, .25f, .25f)); //here we sacle
		glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
 
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
 
			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
 
 
			// Draw the triangles !
			glDrawElements(
				GL_TRIANGLES,      // mode
				indices3.size(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
			
/////////////////// End of rendering of the third object //////


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Draw GUI
		TwDraw();

		char text[256];
		sprintf(text,"%.2f sec", glfwGetTime() );
		printText2D(text, 10, 500, 60);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	CloseEntitySystem();
	
	// Delete the text's VBO, the shader and the texture
	cleanupText2D();

	// Close GUI and OpenGL window, and terminate GLFW
	TwTerminate();
	glfwTerminate();

	return 0;
}

