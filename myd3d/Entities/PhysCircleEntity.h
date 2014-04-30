#pragma once
#include "Entity.h"
#include "../Physics/RigidBody/Circle.h"

class VisualBitmapComponent;
class PhysicsSystem;

class PhysCircleEntity : public Entity
{
public:
    PhysCircleEntity(Scene& scene, std::string& id, PhysicsSystem& physicsSystem);
    ~PhysCircleEntity();

private:
    PhysicsSystem& m_physicsSystem;
    VisualBitmapComponent* m_bmp; 
    Circle* m_circle;
};

