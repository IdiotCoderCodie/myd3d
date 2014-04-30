#include "PhysCircleEntity.h"
#include "../Components/Visual/VisualBitmapComponent.h"
#include "../Scenes/Scene.h"
#include "../Scenes/SceneManager.h"
#include "../Physics/PhysicsSystem.h"

PhysCircleEntity::PhysCircleEntity(Scene& scene, std::string& id, PhysicsSystem& physicsSystem)
: Entity(scene, id),
  m_physicsSystem(physicsSystem),
  m_bmp(new VisualBitmapComponent(scene.GetParent().GetD3DInstance(), L"cement.dds", L"circleStencil.dds", 10, 10, 0, 0)),
  m_circle(0)
{
    this->SetComponent(m_bmp);
    m_circle = &physicsSystem.AddCircle(this, 5.0f, glm::vec2(0.0f), 1.0f);
}


PhysCircleEntity::~PhysCircleEntity()
{
}
