#ifndef __MODEL_H__

#define __MODEL_H__
// Include GLFW
#include <glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "vboindexer.hpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include "entity.h"

/**
 * @brief Mesh object blueprint form handling any 3D models
 * @param filename of the model
 * @param refcount of the model
 * @param vector to hold model verticies
 * @param vector to hold model texeles
 * @param vector to hold model normals
 * @param vector to hold model indicies
 * @param vector to hold model indexed verticies
 * @param vector to hold model indexed texeles
 * @param vector to hold model indexed normals
 * @param assigned vertex buffer
 * @param assigned texel buffer
 * @param assigned normal buffer
 * @param assigned element buffer
 */

typedef struct Model_S
{

	char filename[32];
	int refcount;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;

	GLuint Texture;
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;

}Model_S;

extern GLuint programID;

void initModelSystem();
void closeModelSystem();
Model_S* newModel( const char * path, const char * texture );
void freeModel(Model_S*);
Model_S* loadModel(const char * path);
void drawModel(Model_S* model,GLFWwindow* window, glm::vec3 position, glm::quat orientation);
void drawAll();
void freeModelAll(); 
#endif