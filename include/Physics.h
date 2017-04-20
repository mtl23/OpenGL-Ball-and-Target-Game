#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "entity.h"

	typedef struct Space_S
	{
	
	float step_rate; 
	Entity_S* list;

	}Space_S;

void BoxonBox();
void UpdateSpace(Space_S);
Space_S* newSpace();
void releaseSpace();

#endif