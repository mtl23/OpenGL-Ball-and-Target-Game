#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <graphics_glfw.h>
#include <vector>
#include "Model.h"
#include "objloader.hpp"
using namespace glm;

#include "quaternion_utils.hpp"
#include "Model.h"
#include "simple_logger.h"

Model_S *modelList =NULL;
int modelMax = 10;
void initModelSystem()
{
	modelList = (Model_S*)malloc(sizeof(Model_S)*(modelMax));

	if(modelList == NULL)
	{
		slog("failed to initialize sprite system. STILL NULL");
	}
memset(modelList, 0, sizeof(Model_S)*(modelMax));
slog("model system is go");
}

void freeModel (Model_S *model)
{
  /*first lets check to see if the sprite is still being used.*/
  model->refcount--;
  if(model->refcount== 0)
  {
  strcpy(model->filename,"\0");
  
  }
    if(model->refcount<= 0)
  {
  strcpy(model->filename,"\0");
  memset(model,0,sizeof(model));
  }
 model = NULL;
 
}


void closeModelSystem()
{
  int i;
   if(!modelList)
   {
		return;
   }
  for(i = 0;i < modelMax;i++)
   {
     
      freeModel (&modelList[i]);
	 
   }
slog("model system is closed");
free(modelList);
modelList=NULL;
modelMax = 0;
}


void loadModel(const char * path)
{
}

Model* newModel(const char * path)
{
}



void drawModel(Model*,GLFWwindow* window, glm::vec3 position, glm::vec3 orientation)
{

}
