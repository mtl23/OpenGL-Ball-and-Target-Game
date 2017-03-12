#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "simple_logger.h"
#include "vector.h"
#include "entity.h"
#include "player.h"
#include "model.h"

Player_S newPlayer()
{


	Player_S User;
	User.monkey = EntityNew();
	User.ball = newModel("aiai.obj");
	User.points = 0;
	User.free = freePlayer;
	User.monkey->update = UpdatePlayer;
	User.monkey->draw = DrawPlayer;
	User.monkey->think = ThinkPlayer;
	User.monkey->touch = TouchPlayer;
		
		

return User;
}

void freePlayer(Player_S *player)
{
	
	entityFree(&player->monkey);
	freeModel(player->ball);
	player->points = 0;
	
	
	return;
}



	 