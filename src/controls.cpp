// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <SDL.h>
#include "controls.hpp"
#include "simple_logger.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;
extern  SDL_Window  * __graphics3d_window = NULL;
float speed = .50f; // 3 units / second
float mouseSpeed = 0.005f; 
SDL_Event i;


void computeMatricesFromInputs(){

	
	static double lastTime = 0;

	// Compute time difference between current and last frame
	double currentTime = SDL_GetTicks();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	int  xpos;
	int  ypos;
	SDL_GetMouseState( &xpos, &ypos );
	int HalfScreenX = 512;
	int HalfScreenY = 384;
	// Reset mouse position for next frame
	int DeltaX = xpos - HalfScreenX;
	int DeltaY = ypos - HalfScreenY;
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouseInWindow(__graphics3d_window,HalfScreenX,HalfScreenY);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );


	if ( SDL_PollEvent(&i) ) 
        {

if (i.type == SDL_KEYUP|| SDL_KEYDOWN)
			{
				switch (i.key.keysym.sym)
				{
				case (SDLK_w):
				//position -= glm::normalize (glm::cross (right,up))*speed* deltaTime;
					position += (direction*speed);
				slog("w");
				break;
				
				case (SDLK_s):
				//position += glm::normalize (glm::cross (right,up))*speed* deltaTime;	
					position-= (direction *speed);
				slog("s");
				break;

				case (SDLK_d):
					position += (speed * right);
				slog("d");
				break;
				
				case(SDLK_a):
				position -= (speed * right);
				slog("a");
					break;				
				}
				
			}

	
	float FoV = initialFoV;
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
	}
}