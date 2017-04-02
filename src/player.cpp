#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "simple_logger.h"
#include "vector.h"

#include "player.h"

//extern	Player_S* target1 = newPlayer("crate.obj","greenhair.bmp",glm::vec3(-15.00f, -25.0f, 5.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
//extern	Player_S* target2 = newPlayer("crate3.obj","redhair.bmp",glm::vec3(0.00f, -25.0f,-5.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
//extern	Player_S* target3 = newPlayer("crate2.obj","blondhair.bmp",glm::vec3(15.00f, -25.0f, 5.0f),glm::vec3(.5f,.1f,.7f),glm::quat (0.71f,0.00f,-0.71f,0.00f));
 Player_S* newPlayer(char* path,char* texture, glm::vec3 position,glm::vec3 scale, glm::quat orientation) 
{
	Player_S User;
	memset(&User,0,sizeof(Player_S));
	User.Ent = EntityNew();
	User.Ent->model = newModel(path,texture);
	User.points = 0;
	//User.Ent.update = UpdatePlayer;
	User.Ent->draw = drawModel;
	User.Ent->think = ThinkPlayer;
	//User.Ent.touch = TouchPlayer;
	User.Ent->model->orientation = orientation;
	User.Ent->model->position = position;
	User.Ent->model->scale = scale;

return &User;
}

 int ChangeMap(Player_S* map , int mapnum)
 {
	 slog("got here");
	
		 return mapnum;
 }

void freePlayer(Entity_S *player)
{
	
	entityFree(&player);
	//&player->points = 0;
	return;
}



	 