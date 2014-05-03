#include "PhysCircleEntity.h"
#include "../Components/Visual/VisualBitmapComponent.h"
#include "../Scenes/Scene.h"
#include "../Scenes/SceneManager.h"
#include "../Physics/PhysicsSystem.h"
#include "../Scenes/TerrainDestructionConsts.h"

PhysCircleEntity::PhysCircleEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem)
: Entity(scene, id),
  m_physicsSystem(physicsSystem),
  m_bmp(new VisualBitmapComponent(scene.GetParent().GetD3DInstance(), L"cement.dds", L"circleStencil.dds", 10, 10, 0, 0)),
  m_circle(0)
{
    this->SetComponent(m_bmp);
    m_circle = &physicsSystem.AddCircle(this, 5.0f, glm::vec2(0.0f), 1.0f, 1.0f);
}

PhysCircleEntity::PhysCircleEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem,
                                   float radius, const glm::vec2& pos, const glm::vec2& vel, 
                                   float mass, float elasticity)
: Entity(scene, id),
  m_physicsSystem(physicsSystem),
  m_bmp(new VisualBitmapComponent(scene.GetParent().GetD3DInstance(), PHYS_CIRC_TEX, 
                                  L"circleStencil.dds", radius * 2.0f, radius * 2.0f, 0, 0)),
  m_circle(0)
{
    this->SetComponent(m_bmp);
    this->SetPos(glm::vec3(pos.x, pos.y, 0.0f));
    m_circle = &physicsSystem.AddCircle(this, radius, vel, mass, elasticity);
}


PhysCircleEntity::~PhysCircleEntity()
{
}
