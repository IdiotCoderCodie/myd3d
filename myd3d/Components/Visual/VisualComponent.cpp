#include "VisualComponent.h"


VisualComponent::VisualComponent()
    : m_Shader(0)
{
}


VisualComponent::~VisualComponent()
{
}

void VisualComponent::FamilyID(componentId_t& out) const 
{ 
    out = "VisualComponent"; 
}
