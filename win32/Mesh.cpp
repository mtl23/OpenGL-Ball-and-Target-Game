//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include "mesh.h"
//#include "simple_logger.h"
//
//
//void mesh_free (Mesh **mesh)
//{
//
//	Mesh *m;
//	if (!mesh)
//	{
//	slog("No mesh provided");
//	return;
//	}
//
//	m = *mesh;
//	m->normals = nullptr;
//	m->tris = nullptr;
//	m->texels = nullptr;
//	m->verticies = nullptr;
//	m = nullptr;
//
//
//}
//	
//
//Mesh* mesh_load_from_obj(char * filename)
//	{
//		Mesh *mesh;
//		FILE *file;
//		char buf[512];
//		int v = 0,
//			n = 0, 
//			t =0, 
//			f = 0;
//
//		if (!filename)
//		{
//		slog("cannot load the obj file");
//		return NULL;
//		}
//	
//		file = fopen (filename,"r");
//		
//		if(!file)
//		{
//		slog("Failed to load the file");
//		return NULL;
//		}
//		
//		while(fscanf(file,"&s", buf) != EOF)
//		{
//			switch (buf[0])
//			{
//			case'v':
//				switch (buf[1])
//				{
//				case '\0':
//					fscanf(file,"%f %f %f", &mesh->verticies[v].x,&mesh->verticies[v].y,&mesh->verticies[v].z);
//					v++;
//				break;
//
//				case 'n':
//					fscanf(file,"%f %f %f", &mesh->normals[n].x,&mesh->normals[n].y,&mesh->normals[n].z);
//					n++;
//					break;
//
//				case 't':
//					fscanf(file,"%f %f %f", &mesh->texels[t].x,&mesh->texels[t].y,&mesh->texels[t].z);
//					t++;
//					break;
//				}
//
//			case'f':
//				fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d") ;
//				break;
//			
//			}
//	
//		}
//		mesh = (Mesh *)malloc(sizeof(Mesh));
//		
//		if(!mesh)
//		{
//		slog("failed to create mesh");
//		fclose(file);
//		return NULL;
//		}
//		
//		mesh-> tris = (Triangle *) malloc (sizeof(Triangle)*numfaces);
//		fclose(file);
//}
//	