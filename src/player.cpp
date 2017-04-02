#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "simple_logger.h"
#include "vector.h"
#include "model.h"
#include "player.h"

extern Player_S target1;
extern Player_S target2;
extern Player_S target3;

static int  mapNum = 1;

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

void ChangeMap()
 {
	 
	 mapNum++;
	 if(mapNum>3)
	 {
		 mapNum = 1;
	 }
	 
	 switch(mapNum)

		 {
		 case (1):
		target1.Ent->model->position = glm::vec3(-15.00f, -20.0f, 5.0f);
		target2.Ent->model->position = glm::vec3(0.00f, -20.0f,-5.0f);
		target3.Ent->model->position = glm::vec3(15.00f, -20.0f, 5.0f);
		break;
	 
		 case (2):
		target1.Ent->model->position = glm::vec3(00.00f, -50.0f, -9.0f);
        target2.Ent->model->position = glm::vec3(0.00f, -50.0f,-20.0f);
        target3.Ent->model->position = glm::vec3(0.00f, -50.0f, -32.0f);
		break;

		 case (3):
		target1.Ent->model->position = glm::vec3(-15.00f, -50.0f, -15.0f);
		target2.Ent->model->position = glm::vec3(0.00f, -50.0f,-20.0f);
		target3.Ent->model->position = glm::vec3(15.00f, -50.0f, -25.0f);
		 break;
		 }
	 return;
 }

void freePlayer(Entity_S *player)
{
	
	entityFree(&player);
	//&player->points = 0;
	return;
}



	 