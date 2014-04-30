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

    //float e = (A.GetElasticity() + B.GetElasticity()) / 2.0f;
    float e = glm::min(A.GetElasticity(), B.GetElasticity());

    // Calc impulse scalar
    float j = -(1.0f + e) * velAlongNorm;
    j /= A.GetInvMass() + B.GetInvMass();

    // Apply impulse.
    vec2 impulse = j * point.contactNormal;
    A.SetNewVel(A.GetVel() - (A.GetInvMass()) * impulse);
    B.SetNewVel(B.GetVel() + (B.GetInvMass()) * impulse);

    point.responded = true;

     // Solve friction.
    // Get new relative velocity.
    vec2 rvT = B.GetNewVel() - A.GetNewVel();

    // solve for tangent vec.
    vec2 tangent = rvT - dot(rvT, point.contactNormal) * point.contactNormal;
    if(glm::length2(tangent) > 0.01)
    {
        tangent = glm::normalize(tangent);

        // Solve for magnitude.
        float jt = -dot(rvT, tangent);
        jt = jt / (A.GetInvMass() + B.GetInvMass());

        // Get friction component. of Ff = uFn
        float u = sqrt(A.GetStaticFriction() * A.GetStaticFriction() + B.GetStaticFriction() * B.GetStaticFriction()); 

        // Clamp magnitude.
        vec2 frictionImpulse;
        if(abs(jt) < j * u)
        {
            frictionImpulse = jt * tangent;
        }
        else
        {
            float dynamicFriction = sqrt(A.GetDynamicFriction() * A.GetDynamicFriction() + B.GetDynamicFriction() * B.GetDynamicFriction());
            frictionImpulse = -j * tangent * dynamicFriction;
        }

        A.SetNewVel( A.GetNewVel() - A.GetInvMass() * frictionImpulse);
        B.SetNewVel( B.GetNewVel() + B.GetInvMass() * frictionImpulse);
    }
    // Positional Correction.
    const float percent = 0.4;
    const float slop = 0.05f;

    glm::vec2 correction = glm::max(point.penetration - slop, 0.0f) 
                            / (A.GetInvMass() + B.GetInvMass()) * percent * point.contactNormal;

    A.SetNewPos(A.GetPos() - A.GetInvMass() * correction);
    B.SetNewPos(B.GetPos() + B.GetInvMass() * correction);
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
