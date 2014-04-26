#pragma once
#include "RigidBody.h"

class Circle : public RigidBody
{
public:
    Circle(void);
    ~Circle(void);

    void SetRadius(float radius)    { m_radius = radius; }

    float GetRadius()               { return m_radius; }

    void CollisionWithCircle(Circle* circle, ContactManifold* contactManifold);
    void CollisionWithPolygon(RBPolygon* polygon, ContactManifold* contactManifold);

    void CollisionResponseWithCircle(ManifoldPoint& point);
    void CollisionResponseWithPolygon(ManifoldPoint& point);

private:
    float m_radius;
};

