#pragma once
#include "Entity.h"
#include "../Physics/RigidBody/Circle.h"

class VisualBitmapComponent;
class PhysicsSystem;

class PhysCircleEntity : public Entity
{
public:
    PhysCircleEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem);
    PhysCircleEntity(Scene& scene, const entityId_t& id, PhysicsSystem& physicsSystem,
                     float radius, const glm::vec2& pos, const glm::vec2& vel, 
                     float mass, float elasticity);
    ~PhysCircleEntity();

    float GetElasticity()   { return m_circle->GetElasticity(); }
    glm::vec2 GetVelocity() { return m_circle->GetVel(); }
    float GetRadius()       { return m_circle->GetRadius(); }
    float GetMass()         { return m_circle->GetMass(); }

private:
    PhysicsSystem& m_physicsSystem;
    VisualBitmapComponent* m_bmp; 
    Circle* m_circle;
};
