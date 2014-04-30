#include "PhysAABBEntity.h"
#include "../Scenes/Scene.h"
#include "../Scenes/SceneManager.h"

PhysAABBEntity::PhysAABBEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem)
    : Entity(scene, id),
     m_physicsSystem(physicsSystem),
     m_bmp(new VisualBitmapComponent(scene.GetParent().GetD3DInstance(), L"cement.dds", 20, 20, 0, 0)),
     m_aabb(0)
{
    this->SetComponent(m_bmp);
    m_aabb = &physicsSystem.AddAABB(this, glm::vec2(-20.0f / 2.0f, -20.0f / 2.0f), 
        glm::vec2(20.0f / 2.0f, 20.0f / 2.0f), glm::vec2(0.0f), 1.0f);
}

PhysAABBEntity::PhysAABBEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem,
                               float width, float height, const glm::vec2& pos, const glm::vec2& vel,
                               float mass, float elasticity)
     : Entity(scene, id),
     m_physicsSystem(physicsSystem),
     m_bmp(new VisualBitmapComponent(scene.GetParent().GetD3DInstance(), L"cement.dds", width, 
                                     height, 0, 0)),
     m_aabb(0)
{
    this->SetComponent(m_bmp);
    this->SetPos(glm::vec3(pos.x, pos.y, 0.0f));
    m_aabb = &physicsSystem.AddAABB(this, glm::vec2(-width / 2.0f, -height / 2.0f), 
        glm::vec2(width / 2.0f, height / 2.0f), glm::vec2(0.0f), mass);
}


PhysAABBEntity::~PhysAABBEntity(void)
{
}
