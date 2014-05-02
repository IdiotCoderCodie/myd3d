#include "AABB.h"
#include "Circle.h"
#include "../../glm/gtx/norm.hpp"

AABB::AABB()
{
}


AABB::~AABB()
{
}



void AABB::CollisionWithCircle(Circle& circle, ContactManifold& manifold)
{
    AABB&   A   = *this;
    Circle& B   = circle;

    glm::vec2 n = B.GetNewPos() - A.GetNewPos();

    glm::vec2 closest = n;

    // Calc Half extents along each axis.
    float xExtent = (A.GetMax().x - A.GetMin().x) / 2.0f;
    float yExtent = (A.GetMax().y - A.GetMin().y) / 2.0f;
    //yExtent = -yExtent;
    // Clamp point to edges of AABB.
    closest.x = glm::clamp(closest.x, -xExtent, xExtent);
    closest.y = glm::clamp(closest.y, -yExtent, yExtent);

    // Check if circle is inside the AABB, if so clamp circle's centre to closest edge.
    bool inside = false;
    if(n == closest)
    {
        inside = true;

        // Get closest axis.
        if(glm::abs(n.x) > glm::abs(n.y))
        {
            if(closest.x > 0.0f)
            {
                closest.x = xExtent;
            }
            else
            {
                closest.x = -xExtent;
            }
        }
        else
        {
            // Y Axis shorter.
            if(closest.y > 0.0f)
            {
                closest.y = yExtent;
            }
            else
            {
                closest.y = -yExtent;
            }
        }
    }

    glm::vec2 normal = n - closest;
    float d = glm::length2(normal);
    float r = B.GetRadius();

    // Early escape test, if radius is bigger than dist to closest part, not inside AABB.
    if(d >= r * r  && !inside)
    {
        return;
    }

    d = glm::sqrt(d);

    ManifoldPoint m;
    // IF circle was inside, flip collision normal.
    if(inside)
    {
        n = -n;
    }
    m.contactID1 = &A;
    m.contactID2 = &B;
    m.contactNormal = glm::normalize(normal); // Have to normalize n?
    m.penetration = r - d;
    m.contactPos = A.GetNewPos() + d;
    
    m.responded = false;

    manifold.Add(m);
    
}


void AABB::CollisionWithAABB(AABB& aabb, ContactManifold& contactManifold)
{
    AABB& A = *this;
    AABB& B = aabb;

    glm::vec2 n = B.GetNewPos() - A.GetNewPos();

    // Calculate half extents along x axis for each obj.
    float aExtentX = (A.GetMax().x - A.GetMin().x) / 2.0f;
    float bExtentX = (B.GetMax().x - B.GetMin().x) / 2.0f;

    // Get the overlap.
    float xOverlap = aExtentX + bExtentX - glm::abs(n.x);

    if (xOverlap > 0.0f)
    {
        // Calc half extents along y axis.
        float aExtentY = (A.GetMax().y - A.GetMin().y) / 2.0f;
        float bExtentY = (B.GetMax().y - B.GetMin().y) / 2.0f;

        // Get overlap.
        float yOverlap = aExtentY + bExtentY - glm::abs(n.y);

        if (yOverlap > 0.0f)
        {
            ManifoldPoint m;
            m.contactID1 = &A;
            m.contactID2 = &B;
            // Collision, find axis of least penetration.
            if (xOverlap < yOverlap)
            {
                if (n.x < 0.0f)
                {
                    m.contactNormal = glm::vec2(-1.0f, 0.0f);
                }
                else
                {
                    m.contactNormal = glm::vec2(1.0f, 0.0f);
                }

                m.penetration = xOverlap;
                
                m.contactPos = (A.GetNewPos() + m.contactNormal * aExtentX) 
                                + glm::vec2(0.0f, aExtentY - yOverlap);
            }
            else
            {
                if (n.y < 0.0f)
                {
                    m.contactNormal = glm::vec2(0.0f, -1.0f);
                }
                else
                {
                    m.contactNormal = glm::vec2(0.0f, 1.0f);
                }

                m.penetration = yOverlap;

                m.contactPos = (A.GetNewPos() + m.contactNormal * aExtentY) 
                                + glm::vec2(aExtentX - xOverlap, 0.0f);
                
            }
            m.responded = false;
            contactManifold.Add(m);
        }
    }
}


void AABB::CollisionWithPolygon(RBPolygon& polygon, ContactManifold& manifold)
{

}


void AABB::CollisionResponse(ManifoldPoint& point)
{
    using namespace glm;

    RigidBody& A = *point.contactID1;
    RigidBody& B = *point.contactID2;

    // Relative velocity.
    vec2 rv = B.GetNewVel() - A.GetNewVel();

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
    A.SetNewVel(A.GetNewVel() - (A.GetInvMass()) * impulse);
    B.SetNewVel(B.GetNewVel() + (B.GetInvMass()) * impulse);

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

    A.SetNewPos(A.GetNewPos() - A.GetInvMass() * correction);
    B.SetNewPos(B.GetNewPos() + B.GetInvMass() * correction);
}

//void AABB::CollisionResponseWithCircle(ManifoldPoint& point)
//{
//
//}
//
//
//void AABB::CollisionResponseWithAABB(ManifoldPoint& point)
//{
//
//}
//
//
//void AABB::CollisionResponseWithPolygon(ManifoldPoint* point)
//{
//
//}