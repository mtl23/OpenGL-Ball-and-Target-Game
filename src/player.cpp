#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "simple_logger.h"
#include "vector.h"

#include "player.h"


 Player_S newPlayer()
{

	Player_S User;
	User.Ent = *EntityNew();
	User.Model = newModel("aiai.obj");
	User.points = 50;
	//User.Ent.update = UpdatePlayer;
	//User.Ent.draw = DrawPlayer;
	//User.Ent.think = ThinkPlayer;
	//User.Ent.touch = TouchPlayer;

return User;
}

void freePlayer(Entity_S *player)
{
	
	entityFree(&player);
	freeModel(player->Model);
	//&player->points = 0;
	
	
	return;
}



	 