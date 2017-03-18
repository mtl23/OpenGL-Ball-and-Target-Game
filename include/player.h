#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Model.h"
#include "entity.h"


/**
 * @brief player object that the game use
 * @param position  current position on the map
 * @param accel		current acceleeration
 * @param speed		current speed
 * @param maxaccel  the fastest the car can accerlerate
 * @param minspeed  0
 * @param maxX		player offset --the farthest left the road can go
 * @param minX     player offset --the farthest right the road can go
 * @param playerX where to draw the x coordinate of the road    
 * @param done flag for finishiing a race, remember to set it to 0 again at the strat of the next race
 */

typedef struct Player_S{

int  points;
struct Entity_S Ent;
struct Model_S  Model;		


void(*free) (struct Entity *self); /**cleanup function call on free*<*/

					
			}Player_S;

/**
 * @brief creates player object 
 * @return a new player object
 */
Player_S newPlayer();

/**
 * @brief frees player object data
 */
void freePlayer(Entity_S player);

/**
 * @brief Update the player's speed, acceleration, position, and PlayerX based on inputs 
 */
#endif