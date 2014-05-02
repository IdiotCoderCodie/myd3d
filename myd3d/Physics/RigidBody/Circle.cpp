#include "Circle.h"
#include "../../glm/gtx/norm.hpp"

Circle::Circle(void) : m_radius(1.0f)
{
}


Circle::~Circle(void)
{
}


void Circle::CollisionWithCircle(Circle& circle, ContactManifold& contactManifold)
{
    float r = this->GetRadius() + circle.GetRadius();
    float r2 = r * r;

    glm::vec2 n = circle.GetNewPos() - this->GetNewPos();
    if (glm::length2(n) > r2)
    {
        return;
    }

    // Circles collided... compute manifold point!
    ManifoldPoint newPoint;
    float d = glm::length(n);

    if (d != 0.0f)
    {
        newPoint.penetration = r - d;
        // Convert n to unit vector, already used a sqrt for d, so use it.
        newPoint.contactNormal = n / d;
        newPoint.contactPos = this->GetNewPos() + newPoint.contactNormal * this->GetRadius();
    }
    else
    {
        // Circles have same position... so default penetration to this' radius and normal as up.
        newPoint.penetration = this->GetRadius();
        newPoint.contactPos = this->GetNewPos();
        newPoint.contactNormal = glm::vec2(1.0f, 0.0f);
    }
    newPoint.contactID1 = this;
    newPoint.contactID2 = &circle;
    newPoint.responded = false;

    contactManifold.Add(newPoint);
}


void Circle::CollisionWithAABB(AABB& aabb, ContactManifold& contactManifold)
{
    // TODO: Complete.
}


void Circle::CollisionWithPolygon(RBPolygon& polygon, ContactManifold& contactManifold)
{
    // TODO: complete.
}
