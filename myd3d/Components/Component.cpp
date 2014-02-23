#include "Component.h"


Component::Component(void)
    : m_Parent(0)
{
}


Component::~Component(void)
{
    // Note: Not deleting the parent entity, as a component should not be responsible for this.
    m_Parent = 0;
}

void Component::Update(float time)
{
}

void Component::Draw(D3D& d3d)
{
}