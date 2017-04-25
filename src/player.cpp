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
extern	Player_S ring1;
extern	Player_S ring2;
int mapNum;
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

void Pickup(Player_S* p1)
{
	
	p1->points += 100;
	
	if(p1->points == 100)
	{
		ring1.Ent->model->position.z = 50; 
	}

	if(p1->points == 200)
	{
	ring2.Ent->model->position.z = 50;
	}

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
		target1.Ent->model->position = glm::vec3(-15.00f, -20.0f, -55.0f);
		target2.Ent->model->position = glm::vec3(0.00f, -20.0f,-55.0f);
		target3.Ent->model->position = glm::vec3(15.00f, -20.0f, -55.0f);
		break;
	 
		 case (2):
		target1.Ent->model->position = glm::vec3(0.00f, -50.0f, -45.0f);
        target2.Ent->model->position = glm::vec3(0.00f, -50.0f,-56.0f);
        target3.Ent->model->position = glm::vec3(0.00f, -50.0f, -78.0f);
		break;

		 case (3):
		target1.Ent->model->position = glm::vec3(-15.00f, -50.0f, -55.0f);
		target2.Ent->model->position = glm::vec3(0.00f, -50.0f,-70.0f);
		target3.Ent->model->position = glm::vec3(15.00f, -50.0f, -55.0f);
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



	 