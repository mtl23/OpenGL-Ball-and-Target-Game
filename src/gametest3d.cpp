/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "texture.hpp"
#include "objloader.hpp"
#include "controls.hpp"
#include <vector>
int main(int argc, char *argv[])
{
    //GLuint vao;
   // GLuint triangleBufferObject;
	//GLuint VerteciesBufferObject;
	//GLuint UVBufferObject;
    char bGameLoopRunning = 1;
    SDL_Event e;
	

 //   const float vertexVertices[] = {
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f,-1.0f, 1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	-1.0f,-1.0f, 1.0f,
	//	-1.0f,-1.0f,-1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	-1.0f,-1.0f, 1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	 1.0f,-1.0f,-1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	 1.0f,-1.0f, 1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	 1.0f, 1.0f,-1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f,-1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	 1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	 1.0f,-1.0f, 1.0f,

	//};

	//// Two UV coordinatesfor each vertex. They were created withe Blender.
	//static const GLfloat g_uv_buffer_data[] = { 
	//	0.000059f, 1.0f-0.000004f, 
	//	0.000103f, 1.0f-0.336048f, 
	//	0.335973f, 1.0f-0.335903f, 
	//	1.000023f, 1.0f-0.000013f, 
	//	0.667979f, 1.0f-0.335851f, 
	//	0.999958f, 1.0f-0.336064f, 
	//	0.667979f, 1.0f-0.335851f, 
	//	0.336024f, 1.0f-0.671877f, 
	//	0.667969f, 1.0f-0.671889f, 
	//	1.000023f, 1.0f-0.000013f, 
	//	0.668104f, 1.0f-0.000013f, 
	//	0.667979f, 1.0f-0.335851f, 
	//	0.000059f, 1.0f-0.000004f, 
	//	0.335973f, 1.0f-0.335903f, 
	//	0.336098f, 1.0f-0.000071f, 
	//	0.667979f, 1.0f-0.335851f, 
	//	0.335973f, 1.0f-0.335903f, 
	//	0.336024f, 1.0f-0.671877f, 
	//	1.000004f, 1.0f-0.671847f, 
	//	0.999958f, 1.0f-0.336064f, 
	//	0.667979f, 1.0f-0.335851f, 
	//	0.668104f, 1.0f-0.000013f, 
	//	0.335973f, 1.0f-0.335903f, 
	//	0.667979f, 1.0f-0.335851f, 
	//	0.335973f, 1.0f-0.335903f, 
	//	0.668104f, 1.0f-0.000013f, 
	//	0.336098f, 1.0f-0.000071f, 
	//	0.000103f, 1.0f-0.336048f, 
	//	0.000004f, 1.0f-0.671870f, 
	//	0.336024f, 1.0f-0.671877f, 
	//	0.000103f, 1.0f-0.336048f, 
	//	0.336024f, 1.0f-0.671877f, 
	//	0.335973f, 1.0f-0.335903f, 
	//	0.667969f, 1.0f-0.671889f, 
	//	1.000004f, 1.0f-0.671847f, 
	//	0.667979f, 1.0f-0.335851f
	//};

   
    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d_SDL",33) != 0)
    {
        return -1;
    }

	// Enable depth test
glEnable(GL_DEPTH_TEST);
// Accept fragment if it closer to the camera than the former one
glDepthFunc(GL_LESS);
glEnable(GL_CULL_FACE);

GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

    //Get a refrence to our shaders
	GLuint programID = graphics3d_get_shader_program();
	
	// Get a handle for our "MVP" uniform
	
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	GLuint Texture = loadBMP_custom("aiai.bmp");
	//GLuint Texture = loadDDS("uvtemplate.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");


	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("ballkirby.obj", vertices, uvs, normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind any buffers
    
    slog("glError: %d", glGetError());
    
    while (bGameLoopRunning)
    {
        if ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
                bGameLoopRunning = 0;
            else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                bGameLoopRunning = 0;
			
		}

        glClearColor(0.0,0.0,0.4,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* drawing code in here! */
        glUseProgram(graphics3d_get_shader_program());

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

        glEnableVertexAttribArray(0); //0 is our index, refer to "location = 0" in the vertex shader
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); //bind the buffer we're applying attributes to
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //tell gl (shader!) how to interpret our vertex data
        
		glEnableVertexAttribArray(1); //attribute 1 is for vertex color data
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer); //bind the buffer we're applying attributes to
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);         
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);
        /* drawing code above here! */
        graphics3d_next_frame();
    }
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

    return 0;
}

/*eol@eof*/