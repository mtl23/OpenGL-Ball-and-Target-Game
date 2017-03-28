#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "simple_logger.h"
#include "vector.h"

#include "player.h"


 Player_S* newPlayer(char* path,char* texture, glm::vec3 position, glm::quat orientation) //use a pointer
{

	Player_S User;
	memset(&User,0,sizeof(Player_S));
	User.Ent = EntityNew();
	User.Ent->model = newModel(path,texture);
	User.points = 0;
	//User.Ent.update = UpdatePlayer;
	User.Ent->draw = drawModel;
	//User.Ent.think = ThinkPlayer;
	//User.Ent.touch = TouchPlayer;
	User.Ent->model->orientation = orientation;
	User.Ent->model->position = position;

return &User;
}

void freePlayer(Entity_S *player)
{
	
	entityFree(&player);
	//&player->points = 0;
	return;
}



	 