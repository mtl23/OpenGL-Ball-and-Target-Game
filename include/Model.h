#ifndef __MODEL_H__
// Include GLFW
#include <glfw3.h>
#define __MODEL_H__
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
	glm::quat orientation;


}Model_S;

void initModelSystem();
void closeModelSystem();
Model_S newModel(const char * path);
void freeModel(Model_S*);
Model_S* loadModel(const char * path);
void drawModel(Model_S* model,GLFWwindow* window, glm::vec3 position, glm::vec3 orientation);
void drawAll();
void freeModelAll(); 
#endif