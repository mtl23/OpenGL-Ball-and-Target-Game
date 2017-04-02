#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <graphics_glfw.h>
#include <vector>
#include <iostream>
#include "objloader.hpp"
#include "shader.hpp"
#include "texture.hpp"
using namespace glm;

#include "quaternion_utils.hpp"
#include "Model.h"
#include "shader.hpp"
#include "simple_logger.h"
#include <lodepng.h>

Model_S *modelList =NULL;
int modelMax = 10;
int numModels = 0;


void initModelSystem()
{
	modelList = (Model_S*)malloc(sizeof(Model_S)*(modelMax));

	if(modelList == NULL)
	{
		slog("failed to initialize sprite system. STILL NULL");
	}
memset(modelList, 0, sizeof(Model_S)*(modelMax));
slog("model system is go");
}

void freeModel (Model_S *model)
{

  model->refcount--;
  if(model->refcount== 0)
  {
  strcpy_s(model->filename,"\0");
  
  }
    if(model->refcount<= 0)
  {
  strcpy_s(model->filename,"\0");
  memset(model,0,sizeof(model));
  }
 model = NULL;
 
}


void closeModelSystem()
{
  int i;
   if(!modelList)
   {
		return;
   }
  for(i = 0;i < modelMax;i++)
   {
     
      freeModel (&modelList[i]);
	 
   }
slog("model system is closed");
free(modelList);
modelList=NULL;
modelMax = 0;
}


Model_S* newModel( const char * path, const char * texture )
{
	int i;

	if(!modelList)
			{
			slog("WARNING! no model list to lookup!");
			exit(1);
			}
	 if(numModels + 1 >= modelMax)
			{
			slog("Maximum Models Reached.");
			exit(1);
			}
    if(path == NULL)
  {
   slog("unable to load a vital model");
   exit(1);
  }
  
	numModels++;
  
	for(i = 0;i <= numModels;i++)
  {
     if(!strncmp(path,modelList[i].filename,20))
    {
		slog("already loaded");
		modelList[i].refcount++;
  		return &modelList[i];
    }
}
  memset(&modelList[i],0,sizeof(Model_S));

	modelList[i].refcount++;
    strncpy(modelList[i].filename,path,20);
	bool res = loadOBJ(path, modelList[i].vertices, modelList[i].uvs, modelList[i].normals);
	indexVBO(modelList[i].vertices, modelList[i].uvs, modelList[i].normals, modelList[i].indices, modelList[i].indexed_vertices, modelList[i].indexed_uvs, modelList[i].indexed_normals);

	// Load it into a VBO
	glGenBuffers(1, &modelList[i].vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, modelList[i].vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, modelList[i].indexed_vertices.size() * sizeof(glm::vec3), &modelList[i].indexed_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1, &modelList[i].uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, modelList[i].uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, modelList[i].indexed_uvs.size() * sizeof(glm::vec2), &modelList[i].indexed_uvs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1, &modelList[i].normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, modelList[i].normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, modelList[i].indexed_normals.size() * sizeof(glm::vec3), &modelList[i].indexed_normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &modelList[i].elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelList[i].elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelList[i].indices.size() * sizeof(unsigned short), &modelList[i].indices[0] , GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);


	modelList[i].Texture = loadBMP_custom(texture);

  //std::vector<unsigned char> image; //the raw pixels
  //unsigned width, height;

  //decode PNG
  //modelList[i].Texture = lodepng::decode(image, width, height, texture);
	
  //if there's an error, display it
  //if(!modelList[i].Texture) std::cout << "decoder error " << modelList[i].Texture << ": " << lodepng_error_text(modelList[i].Texture) << " "<<texture << std::endl;
  //else{
  //std::cout << "decoder error " << modelList[i].Texture << ": " << lodepng_error_text(modelList[i].Texture) << " "<< texture << std::endl;
  //}

	return &modelList[i];
}


void drawModel(Model_S* model ,GLFWwindow* window, glm::vec3 position, glm::quat orientation)
{
		// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/StandardShading.vertexshader", "shaders/StandardTransparentShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(model->Texture, 0);
		glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
			glm::mat4 ViewMatrix = glm::lookAt(
			glm::vec3( 0, 20, 37 ), // Camera is here
			glm::vec3( 0, 0, 0 ), // and looks here
			glm::vec3( 0, 1, 0 )  // Head is up (set to 0,-1,0 to look upside-down)
		);

			// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, model->vertexbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, model->uvbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, model->normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->elementbuffer);
		glm::vec3 lightPos = glm::vec3(3,50,5);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glm::mat4 RotationMatrix = mat4_cast(model->orientation);
		glm::mat4 TranslationMatrix = translate(mat4(), model->position); // places into position
		glm::mat4 ScalingMatrix = scale(mat4(), model->scale); //here we sacle
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
				model->indices.size(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDeleteProgram(programID);
		glDeleteTextures(1, &TextureID);

}
