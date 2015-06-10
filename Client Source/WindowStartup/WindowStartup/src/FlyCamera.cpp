#include "FlyCamera.h"


FlyCamera::FlyCamera()
{
	xPosition = -23;
	yPosition = 60;
	zPosition = 20;

	cameraPosition = vec3(xPosition,yPosition,zPosition);
	cameraDirection = vec3(47,-39,19);
	
	Speed = 2;

}

FlyCamera::~FlyCamera()
{

}

void FlyCamera::update(float deltatime)
{

	forward = vec3(worldTransform[2].x, worldTransform[2].y,worldTransform[2].z);
	up = vec3(worldTransform[1].x, worldTransform[1].y,worldTransform[1].z);
	right = vec3(worldTransform[0].x, worldTransform[0].y,worldTransform[0].z);


	cameraPosition = vec3(xPosition, yPosition, zPosition);

	if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		//xPosition += .1f;
		cameraDirection += right * deltatime * Speed;
		
	
	}
	
	if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		//xPosition -= .1f;
		cameraDirection -= right * deltatime * Speed;
	}
	
	if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		//zPosition -= .1f;
		cameraDirection -= forward * deltatime ;
		xPosition -= forward.x * Speed * 0.1f;
		yPosition -= forward.y * Speed * 0.1f;
		zPosition -= forward.z * Speed * 0.1f;
	}
	
	if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		//zPosition += .1f;
		cameraDirection += forward * deltatime;
		xPosition += forward.x * Speed * 0.1f;
		yPosition += forward.y * Speed * 0.1f;
		zPosition += forward.z * Speed * 0.1f;
	}
	
	if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
	{
		//zPosition += .1f;
		cameraDirection -= up * deltatime * Speed;
		
	}
	
	if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		//zPosition += .1f;
		cameraDirection += up * deltatime * Speed;
	
	}

	
	//viewmatrix = glm::lookAt(vec3(xPosition,yPosition,zPosition), cameraPosition + cameraDirection, vec3(0,1,0));
	viewmatrix = glm::lookAt(vec3(xPosition,yPosition,zPosition),  cameraDirection, vec3(0,.25f,0));
	ProjectionView = glm::perspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.f);

	//cameraDirection = vec3(0);
}

void FlyCamera::setPerspective(float fieldofView, float aspectRatio, float near, float far)
{
	
}

void FlyCamera::setLookAt(vec3 from, vec3 to, vec3 up)
{
	viewmatrix = glm::lookAt(from, to, up);
}

mat4 FlyCamera::getWorldTransform()
{
		
	worldTransform = glm::inverse(viewmatrix);
	return worldTransform;
}


mat4 FlyCamera::getView()
{
	return viewmatrix;
}

mat4 FlyCamera::getProjectionTransform()
{
	ProjectionViewTransform = ProjectionView * viewmatrix;
	return ProjectionViewTransform;
}

mat4 FlyCamera::getProjectionView()
{
	return ProjectionView;
}

vec3 FlyCamera::getPosition()
{
	return cameraPosition;
}

vec3 FlyCamera::pickAgainstPlane(float x, float y, const glm::vec4& plane) const
{
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= ProjectionView[0][0];
	screenPos.y /= ProjectionView[1][1];

	glm::vec3 dir = glm::normalize(worldTransform * glm::vec4(screenPos, 0)).xyz();

	float d = (plane.w - glm::dot(worldTransform[3].xyz(), plane.xyz()) / glm::dot(dir, plane.xyz()));

	return worldTransform[3].xyz() + dir * d;

}