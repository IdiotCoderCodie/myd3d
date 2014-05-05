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
    Circle& GetCircle()     { return *m_circle; }

    void SetElasticity(float elasticity)    { m_circle->SetElasticity(elasticity); }
    void SetVelocity(const glm::vec2& vel)  { m_circle->SetVel(vel); }
    void SetRadius(float radius)            { m_circle->SetRadius(radius); }
    void SetMass(float mass)                { m_circle->SetMass(mass); }

    void DetachPhysicsObject();

    void Update(double time);

private:
    PhysicsSystem& m_physicsSystem;
    VisualBitmapComponent* m_bmp; 
    Circle* m_circle;
};
