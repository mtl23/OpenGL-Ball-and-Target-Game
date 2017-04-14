#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Model.h"
#include "entity.h"


/**
 * @brief player object that the game use
 * @param int points for recording the player's score
 * @param Entity for position,salce, rotation, logic and draw functions,meshes etc
 * @param (*free) here we can assign our own cleanup function if necessary
 */

typedef struct Player_S{

int  points;
struct Entity_S *Ent;
void(*free) (struct Entity *self); /**cleanup function call on free*<*/

					
			}Player_S;

/**
 * @brief creates player object 
 * @param path to the mesh file
 * @param path to the texture file
 * @param position in worldspace
 * @param scale in worldspace
 * @param orientation in worldspace
 * @return a new player object
 */
Player_S* newPlayer(char* path,char* texture, glm::vec3 position,glm::vec3 scale, glm::quat orientation);

void ChangeMap();

/**
 * @brief frees player object data
 * @param player ot be freed
 */
void freePlayer(Entity_S player);

/**
 * @brief Update the player's speed, acceleration, position, and PlayerX based on inputs 
 */

void Pickup(Player_S* p1);

#endif