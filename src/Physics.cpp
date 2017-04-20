#include "Physics.h"

extern Entity_S *entityList;

Space_S* newSpace()
{
	Space_S* temp;
	temp = (Space_S*)malloc(sizeof(Space_S));
	//memset(&temp,0,sizeof(Space_S));
	temp->list = NULL;
	temp->step_rate = 0.00f;

	temp->list = entityList;
	temp->step_rate = .1f;
	
	return temp;
}