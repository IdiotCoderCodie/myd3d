#pragma once
#include "Entity.h"
#include "../Physics/PhysicsSystem.h"
#include "../Physics/RigidBody/AABB.h"
#include "../Components/Visual/VisualBitmapComponent.h"

class PhysAABBEntity : public Entity
{
public:
    PhysAABBEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem);
    PhysAABBEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem,
                   float width, float height, const glm::vec2& pos, const glm::vec2& vel,
                   float mass, float elasticity);

    ~PhysAABBEntity(void);

    float GetElasticity()       { return m_aabb->GetElasticity(); }
    glm::vec2 GetVelocity()     { return m_aabb->GetVel(); }
    const glm::vec2& GetMin()   { return m_aabb->GetMin(); }
    const glm::vec2& GetMax()   { return m_aabb->GetMax(); }
    float GetMass()             { return m_aabb->GetMass(); }
    AABB& GetAABB()             { return *m_aabb; }

    void SetElasticity(float elasticity)    { m_aabb->SetElasticity(elasticity); }
    void SetVelocity(const glm::vec2& vel)  { m_aabb->SetVel(vel); }
    void SetMin(const glm::vec2& min)       { m_aabb->SetMin(min); }
    void SetMax(const glm::vec2& max)       { m_aabb->SetMax(max); }
    void SetMass(float mass)                { m_aabb->SetMass(mass); }

    void DetachPhysicsObject();

    void Update(double time);

private:
    PhysicsSystem& m_physicsSystem;
    VisualBitmapComponent* m_bmp;
    AABB* m_aabb;
};

