#include "Circle.h"


Circle::Circle(void) : m_radius(1.0f)
{
}


Circle::~Circle(void)
{
}

void Circle::CollisionWithCircle(Circle* circle, ContactManifold* contactManifold)
{
    if((circle->GetNewPos() - this->GetNewPos()).length() < circle->GetRadius() + this->GetRadius())
    {
        ManifoldPoint newPoint;
        newPoint.contactID1     = this;
        newPoint.contactID2     = circle;
        newPoint.contactNormal  = glm::normalize(this->GetNewPos() - circle->GetNewPos());
        newPoint.contactPoint   = circle->GetNewPos() + (circle->GetRadius() * newPoint.contactNormal);
        newPoint.responded      = false;
        contactManifold->Add(newPoint);
    }
}

void Circle::CollisionWithSquare(RigidBody* circle, ContactManifold* contactManifold)
{
    // TODO: complete.
}

void Circle::CollisionResponseWithCircle(ManifoldPoint& point)
{
    // TODO: complete.
}

void Circle::CollisionResponseWithSquare(ManifoldPoint& point)
{
    // TODO: complete.
}
