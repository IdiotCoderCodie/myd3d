#include "AABB.h"

AABB::AABB()
{
}


AABB::~AABB()
{
}



void AABB::CollisionWithCircle(Circle& circle, ContactManifold& manifold)
{
}


void AABB::CollisionWithAABB(AABB& aabb, ContactManifold& contactManifold)
{
    AABB& A = *this;
    AABB& B = aabb;

    glm::vec2 n = B.GetPos() - A.GetPos();

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
            if (xOverlap > yOverlap)
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
                
                m.contactPos = (A.GetPos() + m.contactNormal * aExtentX) 
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

                m.contactPos = (A.GetPos() + m.contactNormal * aExtentY) 
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