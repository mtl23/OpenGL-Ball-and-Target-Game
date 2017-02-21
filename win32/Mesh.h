#ifndef __MESH_H__
#define __MESH_H__
#include "vector.h"
#include "Mesh.h"
typedef struct 
{
	int v;
	int vn;
	int vt;
} triCorner;

	typedef struct
{
	triCorner[3];
}Triangle;

typedef struct 
{
	Vector3D * verticies;
	Vector2D * texels;
	Vector3D * normals;
	Triangle  * tris;
} Mesh;

Mesh * mesh_load_from_obj (char * filename);

void mesh_free (Mesh **mesh);

#endif