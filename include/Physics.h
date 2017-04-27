#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "entity.h"

#define BTQUAT2GLMQUAT(GLM,BT) (GLM).x = (BT).x(); \
							(GLM).y = (BT).y(); \
							(GLM).z = (BT).z(); \
							(GLM).w = (BT).w(); 


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