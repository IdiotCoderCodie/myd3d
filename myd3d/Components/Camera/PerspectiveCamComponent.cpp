#include "PerspectiveCamComponent.h"
#include "../../glm/gtc/matrix_transform.hpp"

PerspectiveCamComponent::PerspectiveCamComponent(void)
    : m_fov(45.0f), m_aspect(600.0f / 400.0f), m_near(0.01f), m_far(1000.0f)
{
    CameraComponent::SetProjMatrix(glm::perspective(m_fov, m_aspect, m_near, m_far));
}


PerspectiveCamComponent::PerspectiveCamComponent(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
{   
    glm::mat4 ogl_to_d3d = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
    ogl_to_d3d = glm::scale(ogl_to_d3d, glm::vec3(1.0f, 1.0f, 0.5f));

    glm::mat4 projection = ogl_to_d3d * glm::perspective(m_fov, m_aspect, m_near, m_far);

    CameraComponent::SetProjMatrix(projection);
}


PerspectiveCamComponent::~PerspectiveCamComponent(void)
{
}

void PerspectiveCamComponent::ComponentID(componentId_t& out) const 
{ 
    out = "PerspectiveCameraComponent"; 
}