#include "CameraComponent.h"
#include "../../Entities/Entity.h"

CameraComponent::CameraComponent()
    : m_ViewMatrix(), m_ProjMatrix()
{
}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::FamilyID(componentId_t& out) const 
{ 
    out = "CameraComponent"; 
}

void CameraComponent::Update(float timeElapsed)
{
    // Update view matrix according to the Frame of the object it's attached to! 
    const Frame& transform = GetParent().GetTransform();
    m_ViewMatrix = glm::lookAt(transform.GetPosition(),
                               transform.GetPosition() + transform.GetForward(),
                               transform.GetUp());
}