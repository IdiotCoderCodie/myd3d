#include "RBPolygon.h"


RBPolygon::RBPolygon(void)
{
}

RBPolygon::RBPolygon(std::vector<glm::vec2>& vertices)
    : m_vertices(vertices)
{
}


RBPolygon::~RBPolygon(void)
{
}


void RBPolygon::CollisionWithCircle(Circle* circle, ContactManifold* ContactManifold)
{

}


void RBPolygon::CollisionWithPolygon(RBPolygon* polygon, ContactManifold* ContactManifold)
{

}


bool RBPolygon::Intersect(RBPolygon& A, RBPolygon& B)
{
    int j, i;

    // Check all axis from A edges.
    // Start with 1st vertex, and last to get first edge.
    for(j = A.GetNumVertices() - 1, i = 0; i < A.GetNumVertices(); j = i, i++)
    {
        glm::vec2 E = A[i] - A[j];
        glm::vec2 N(-E.y, E.x);

        if(RBPolygon::AxisSeparatePolygons(N, A, B))
        {
            return false;
        }
    }

    // Check all axis from B edges.
    for(j = B.GetNumVertices() - 1, i = 0; i < B.GetNumVertices(); j = i, i++)
    {
        glm::vec2 E = B[i] - B[j];
        glm::vec2 N(-E.y, E.x);

        if(RBPolygon::AxisSeparatePolygons(N, A, B))
        {
            return false;
        }
    }
}


bool RBPolygon::AxisSeparatePolygons(const glm::vec2& axis, const RBPolygon& A, const RBPolygon& B)
{
    return false;
}