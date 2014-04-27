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
    float mina, maxa;
    float minb, maxb;

    CalculateInterval(axis, A, mina, maxa);
    CalculateInterval(axis, B, minb, maxb);

    if(mina > maxb || minb > maxa)
        return true;

    return false;
}


void RBPolygon::CalculateInterval(const glm::vec2& axis, const RBPolygon& P, float& min, float& max)
{
    // Projects the polygon onto the axis, getting minimum and maximum of the interval.

    float d = glm::dot(axis, P[0]);
    min = max = d;
    for(int i = 0; i < P.GetNumVertices(); i++)
    {
        float d2 = glm::dot(P[i], axis);
        if(d2 < min)
        {
            min = d2;
        }
        else if(d2 > max)
        {
            max = d2;
        }
    }
}