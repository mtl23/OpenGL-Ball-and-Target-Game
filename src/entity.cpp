#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "simple_logger.h"
#include "graphics_glfw.h"
#include "objloader.hpp"

extern GLFWwindow* window ;

Entity *entityList = NULL;
int numEntity = 0;
int entityMax = 90;


void InitEntitySystem(int EntityMax)
{

	if(numEntity == entityMax)
	{
	slog("Cannot init entities");
	return;
	}
		entityList = (Entity*)malloc(sizeof(Entity)*(entityMax));

	if(entityList == NULL)
	{
		slog("failed to initialize entity system. STILL NULL");
	}
memset(entityList, 0, sizeof(Entity)*(entityMax));
slog("Entity system is go");

atexit(CloseEntitySystem);
}


void CloseEntitySystem()

{
   if(!entityList)
   {
		return;
   }
  
slog("entity system is closed");
free(entityList);
entityList=NULL;
numEntity = 0;
}


Entity* EntityNew()
{
	int i;
	for( i= 0 ; i < entityMax; i++ )
	{
		if(entityList[i].inuse)
		{
			continue;
		}
		memset(&entityList[i],0,sizeof(Entity));
		entityList[i].inuse = 1;
		slog("NEW ENTITY");
		if(!&entityList[i])
		{
			slog("ENTITY ALLOCATION FAILED");
			return NULL;
		}

		return &entityList[i];
	}
}



void entityFree(Entity **entity)
{
	(*entity)->inuse= 0;
	*entity = NULL;
  
}


Entity *entityLoad(char *filename, glm::vec2 position) //should load a object entity if needed
{
  int i;
  Entity *temp;
  temp = EntityNew();

  /*makesure we have the room for a entity*/
  if(numEntity + 1 >= entityMax)
  {
      slog( "Maximum Entities Reached.");
        exit(1);
  }
  /*if its not already in memory, then load it.*/
  
  numEntity++;
 // temp->sprite = temp_spr;
  //strncpy(temp->filename, temp_spr.filename,20);
  for(i = 0;i <= numEntity;i++)
  {
    if(entityList[i].inuse)break;
  }

  if(&temp == NULL)
  {
  slog("unable to load a vital sprite");
  exit(0);
  }
 slog("loaded an entity for the first time");


 temp->inuse = 1;
  return temp;
}

void entityDraw(Entity *entity,GLFWwindow* window, glm::vec3 position, glm::vec3 orientation)
{
	if((!entity)||(!window))
	{
		
		slog("no valid entity or window");
		return;
	}

	if(!&entity->Model)
	{
	
		slog("no valid entity sprite");
		return;
	}
	drawModel(&entity->Model,window,position,orientation);
}

void entityThinkAll()
{
 int i;
 for(i=0;i<entityMax;i++)
 {
	 if(!entityList[i].inuse)
	 {
		continue;
	 }
	 if(!entityList[i].think)
	 {
		continue;
	 }
	 entityList[i].think(&entityList[i]);

 }
}

void entityUpdateAll()
{
 int i;
 for(i=0;i<entityMax;i++)
 {
	 if(!entityList[i].inuse)
	 {
		continue;
	 }
	 if(!entityList[i].update)
	 {
		continue;
	 }
	 entityList[i].update(&entityList[i]);

	}

}

void entityDrawAll()
{
 int i;
 for(i=0;i<entityMax;i++)
 {
  if(!entityList[i].inuse)
	 {
		continue;
	 }
  if(!entityList[i].draw)
	 {
		continue;
	 }
	 entityList[i].draw(&entityList[i], window);
	}

 }

void ThinkPlayer(Entity_S* user)
{}
void TouchPlayer(Entity_S* user, Entity_S* other)
{}

void DrawPlayer(Entity_S* user, GLFWwindow* window)
{}

void UpdatePlayer(Entity_S* user)
{
	
}


void ThinkObstacle(Entity_S* obstacle)
{}
void TouchObstacle(Entity_S* obstacle, Entity_S* user)
{}

void DrawObstacle(Entity_S* obstacle, GLFWwindow* window)
{}

void UpdateObstacle(Entity_S* obstacles)
{
	
}
