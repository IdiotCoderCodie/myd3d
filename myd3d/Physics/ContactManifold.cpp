#include "ContactManifold.h"

using namespace glm;

ContactManifold::ContactManifold(void)
    : m_numOfPoints(0)
{
}


ContactManifold::~ContactManifold(void)
{
}


void ContactManifold::Add(ManifoldPoint point)
{
    float dist = 0.0f;
    for(int collision = 0; collision < m_numOfPoints; ++collision)
    {
        //vec2 colNormal = (m_points
    }

}
