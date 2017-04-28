#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "simple_logger.h"
#include "vector.h"
#include "model.h"
#include "player.h"

btQuaternion targetRot = btQuaternion(0.71f, 0.00f, -0.71f, 0.00f);

extern Player_S target1;
extern Player_S target2;
extern Player_S target3;
extern	Player_S ring1;
extern	Player_S ring2;
extern btRigidBody *rigidBody3;
extern btRigidBody *rigidBody4;
extern btRigidBody *rigidBody5;
extern btRigidBody *rigidBody6;
extern btRigidBody *rigidBody7;
extern btVector3 t1;
extern btVector3 t2;
extern btVector3 t3;
extern btDiscreteDynamicsWorld* dynamicsWorld;
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
	
	if (p1->Ent->model->position.x <= 0)  // if(p1->Ent->model->position.x <= 0)
	{
		ring2.Ent->model->position.y = 570;
		ring2.Ent->model->position.z = 50; 
		rigidBody6->setWorldTransform(btTransform(
			btQuaternion(-.06, -.71, .08, .71),
			btVector3(0, 80000, -15)
		));
	}

	if (p1->Ent->model->position.x >= 0) // if(p1->Ent->model->position.x >= 0)
	{
	ring1.Ent->model->position.z = 50;
	ring1.Ent->model->position.y = 570;
	rigidBody7->setWorldTransform(btTransform(
		btQuaternion(-.06, -.71, .08, .71),
		btVector3(0, 80000, -15)
	));
	}

}

void ChangeMap()
 {


	slog("i",mapNum);
	 mapNum++;
	 if(mapNum>3)
	 {
		 mapNum = 1;
	 }
	 
	 switch(mapNum)

		 {
		 case (1):
		target1.Ent->model->position = glm::vec3(-35.00f, -50.0f, -55.0f);
		rigidBody3->setWorldTransform(btTransform(btQuaternion(targetRot), t1 ));
		target2.Ent->model->position = glm::vec3(0.00f, -50.0f,-60.0f);
		rigidBody4->setWorldTransform(btTransform(btQuaternion(targetRot), t2));
		target3.Ent->model->position = glm::vec3(35.00f, -50.0f, -65.0f);
		rigidBody5->setWorldTransform(btTransform(btQuaternion(targetRot), t3));
		break;
	 
		 case (2):
			 target1.Ent->model->position = glm::vec3(0.00f, -50.0f, -40.0f);
			 rigidBody3->setWorldTransform ( btTransform(btQuaternion(targetRot),(btVector3(0.00f, -50.0f, -40.0f))));
			 target2.Ent->model->position = glm::vec3(0.00f, -50.0f, -60.0f);
			 rigidBody4->setWorldTransform (btTransform(btQuaternion(targetRot), (btVector3(0.00f, -50.0f, -60.0f))));
			 target3.Ent->model->position = glm::vec3(0.00f, -50.0f, -80.0f);
			 rigidBody5->setWorldTransform  ( btTransform(btQuaternion(targetRot), (btVector3(0.00f, -50.0f, -80.0f))));
			 break;

		 case (3):
			 target1.Ent->model->position = glm::vec3(40.00f, -50.0f, -40.0f);
			 rigidBody3->setWorldTransform(btTransform(btQuaternion(targetRot), (btVector3(40.00f, -50.0f, -40.0f))));
			 target2.Ent->model->position = glm::vec3(-40.00f, -50.0f, -40.0f);
			 rigidBody4->setWorldTransform(btTransform(btQuaternion(targetRot), (btVector3(-40.00f, -50.0f, -40.0f))));
			 target3.Ent->model->position = glm::vec3(0.00f, -50.0f, -60.0f);
			 rigidBody5->setWorldTransform(btTransform(btQuaternion(targetRot), (btVector3(0.00f, -50.0f, -60.0f))));
		 break;
		 }


	 rigidBody7->setWorldTransform(btTransform(
		 btQuaternion(0.71f, 0.00f, -0.71f, 0.00f),
		 btVector3(5, -8, -20)
	 ));
	 ring1.Ent->model->position = glm::vec3(5, -8, -20);

	 rigidBody6->setWorldTransform(btTransform(
		 btQuaternion(0.71f, 0.00f, -0.71f, 0.00f),
		 btVector3(-5, -8, -20)
	 ));
	 ring2.Ent->model->position = glm::vec3(-5,-8,-20);

	 return;
 }

void freePlayer(Entity_S *player)
{
	
	entityFree(&player);
	//&player->points = 0;
	return;
}



	 