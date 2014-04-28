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

    glm::vec2 n = circle.GetPos() - this->GetPos();
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
        newPoint.contactPos = this->GetPos() + newPoint.contactNormal * this->GetRadius();
    }
    else
    {
        // Circles have same position... so default penetration to this' radius and normal as up.
        newPoint.penetration = this->GetRadius();
        newPoint.contactPos = this->GetPos();
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


void Circle::CollisionResponse(ManifoldPoint& point)
{
    using namespace glm;

    RigidBody& A = *point.contactID1;
    RigidBody& B = *point.contactID2;

    // Relative velocity.
    vec2 rv = B.GetVel() - A.GetVel();

    // Get in terms of normal direction.
    float velAlongNorm = glm::dot(rv, point.contactNormal);

    // Check if velocities are moving apart.
    if (velAlongNorm > 0.0f)
        return;

    float e = glm::min(A.GetElasticity(), B.GetElasticity());

    // Calc impulse scalar
    float j = -(1.0f + e) * velAlongNorm;
    j /= A.GetInvMass() + B.GetInvMass();

    // Apply impulse.
    vec2 impulse = j * point.contactNormal;
    A.SetNewVel(A.GetVel() - (A.GetInvMass()) * impulse);
    B.SetNewVel(B.GetVel() + (B.GetInvMass()) * impulse);

    point.responded = true;
}

//
//void Circle::CollisionResponseWithCircle(ManifoldPoint& point)
//{
//    using namespace glm;
//
//    RigidBody& A = *point.contactID1;
//    RigidBody& B = *point.contactID2;
//
//    // Relative velocity.
//    vec2 rv = B.GetVel() - A.GetVel();
//
//    // Get in terms of normal direction.
//    float velAlongNorm = glm::dot(rv, point.contactNormal);
//
//    // Check if velocities are moving apart.
//    if (velAlongNorm > 0.0f)
//        return;
//
//    float e = glm::min(A.GetElasticity(), B.GetElasticity());
//
//    // Calc impulse scalar
//    float j = -(1.0f + e) * velAlongNorm;
//    j /= A.GetInvMass() + B.GetInvMass();
//
//    // Apply impulse.
//    vec2 impulse = j * point.contactNormal;
//    A.SetNewVel(A.GetVel() - (A.GetInvMass()) * impulse);
//    B.SetNewVel(B.GetVel() + (B.GetInvMass()) * impulse);
//
//	point.responded = true;
//}
//
//void Circle::CollisionResponseWithPolygon(ManifoldPoint& point)
//{
//    // TODO: complete.
//}
