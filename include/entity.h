#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <glfw3.h>

#include <glm/glm.hpp>
/**
*@brief The data  structure for Entities 
*/

#include "Model.h"
typedef struct Entity_S
{
	int inuse; 
	Model_S* model;
	glm::vec3 speed;       
	glm::vec3 accel;        
	glm::vec3 maxaccel;     
	glm::vec3 minaccel;		
	glm::vec3 minspeed;        
	glm::vec3 maxspeed;        

	int next_think;/**time index for next_think*/
	int think_rate; /**how often to run think*/
	void(*draw) (Model_S* model ,GLFWwindow* window, glm::vec3 position, glm::quat orientation); /**logic*<*/
	void(*think) (struct Entity_S *self); /**logic*<*/
	void(*update) (struct Entity_S *self); /**physics*<*/
	void(*touch) (struct Entity_S *self, struct Entity_S *other); /**collsions*<*/
	void(*free) (struct Entity_S *self); /**cleanup function call on free*<*/

}Entity_S;


/**
*
*@brief creates the entity system should be used once at game start
*/
void InitEntitySystem(int EntityMax);
/**
*
*@brief closes the entity system should be used once at game end
*/
void CloseEntitySystem();

/**
*
*@brief creates a single new Entity object
*@return an Entity type object
*/
Entity_S* EntityNew();
/**
*
*@brief Frees up a single Entity
*/
void entityFree(Entity_S **entity);

/**
*@brief Draws an Entity, it takes a render so it can be used with spriteDraw()
*@param Entity: The entity you wish to be drawn
*@param renderer: The renderer to draw the Entity's sprite onto
*@param position: A glm 3D vector used to state position on the screen
*/
void entityDraw(Entity_S *entity,GLFWwindow* window, int frame, glm::vec3 position, glm::vec3 orientation);

/**
*
*@brief Goes through the Entity List and calls each entity's draw fucntion
*/
void entityDrawAll();
/**
*
*@brief Goes through the Entity List and calls each entity's Update fucntion
*/
void entityUpdateAll();
/**
*
*@brief Goes through the Entity List and calls each entity's think fucntion
*/
void entityThinkAll();

void UpdatePlayer(struct Entity_S* user); 

void DrawPlayer(struct Entity_S* user, GLFWwindow* window); //make this a ptr functiion

void TouchPlayer(struct Entity_S* user, Entity_S* other);

void ThinkPlayer(struct Entity_S* user); //make this a ptr functiion

void UpdateObstacle(struct Entity_S* obstacle); 

void DrawObstacle(struct Entity_S* obstacle, GLFWwindow* window); //make this a ptr functiion

void TouchObstacle(struct Entity_S* obstacle, Entity_S* user);

void ThinkObstacle(struct Entity_S* obstacle); //make this a ptr functiion
#endif