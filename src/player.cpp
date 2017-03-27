#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "simple_logger.h"
#include "vector.h"

#include "player.h"


 Player_S* newPlayer() //use a pointer
{

	Player_S User;
	memset(&User,0,sizeof(Player_S));
	User.Ent = EntityNew();
	User.Ent->model = newModel("aiai.obj");
	User.points = 0;
	//User.Ent.update = UpdatePlayer;
	User.Ent->draw = drawModel;
	//User.Ent.think = ThinkPlayer;
	//User.Ent.touch = TouchPlayer;
	User.Ent->model->orientation = glm::quat(0.71f,0.00f,-0.71f,0.00f);
	User.Ent->model->position = glm::vec3 (-5.00f, 5.0f, 15.0f);

return &User;
}

void freePlayer(Entity_S *player)
{
	
	entityFree(&player);
	//&player->points = 0;
	return;
}



	 