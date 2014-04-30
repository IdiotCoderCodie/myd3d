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

private:
    PhysicsSystem& m_physicsSystem;
    VisualBitmapComponent* m_bmp; 
    Circle* m_circle;
};

