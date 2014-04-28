#include "ContactManifold.h"
#include "RigidBody\Circle.h"

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
        if (distance(m_points[collision].contactPos, point.contactPos) < 0.01f)
        {
            return;
        }
    }

	m_points[m_numOfPoints] = point;
	++m_numOfPoints;
}


void ContactManifold::Assess()
{
	// This function removes any collision points where the collision is no longer occurring.
	//bool loop = true;
	//while (loop)
	//{
	//	loop = false;
	//	for (int collision = 0; collision < m_numOfPoints; collision++)
	//	{
 //           RigidBody& A = *m_points[collision].contactID1;
 //           RigidBody& B = *m_points[collision].contactID2;

	//		Circle* circle1 = m_points[collision].contactID1;
	//		Circle* circle2 = m_points[collision].contactID2;

	//		if (distance(circle1->GetPos(), circle2->GetPos()) 
	//			> circle1->GetRadius() + circle2->GetRadius())
	//		{
	//			// No collision.
	//			for (int n = collision; n < m_numOfPoints - 1; ++n)
	//			{
	//				m_points[n] = m_points[n + 1];
	//			}
	//			--m_numOfPoints;
	//			loop = true;
	//		}
	//	}
	//}
}
