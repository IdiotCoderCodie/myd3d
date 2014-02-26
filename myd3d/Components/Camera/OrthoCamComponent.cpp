#include "OrthoCamComponent.h"

OrthoCamComponent::OrthoCamComponent()
{
}


OrthoCamComponent::OrthoCamComponent(float left, float right, float bottom, float top)
{
	glm::mat4 ogl_to_d3d = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
	ogl_to_d3d = glm::scale(ogl_to_d3d, glm::vec3(1.0f, 1.0f, 0.5f));

	CameraComponent::SetProjMatrix(ogl_to_d3d * glm::ortho(left, right, bottom, top));
}

OrthoCamComponent::~OrthoCamComponent()
{
}


void OrthoCamComponent::ComponentID(componentId_t& out) const
{
	out = "OrthographicCameraComponent";
}
