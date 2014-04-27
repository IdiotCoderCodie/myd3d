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
        RigidBody& A = *m_points[collision].contactID1;
		// Check this point is not already in the manifold 
		// (by checking if it's position is really close to any others).
		glm::vec2 colNormal = normalize( (m_points[collision].contactID2->GetNewPos() 
										- m_points[collision].contactID1->GetNewPos()));

		/*glm::vec2 colPos = m_points[collision].contactID1->GetNewPos()
							+ colNormal * m_points[collision].contactID1->GetRadius();*/

		/*dist = distance(colPos, point.contactPoint);
		if (dist < 0.01f)
		{
			return;
		}*/
    }

	m_points[m_numOfPoints] = point;
	++m_numOfPoints;
}


void ContactManifold::Assess()
{
	// This function removes any collision points where the collision is no longer occurring.
	bool loop = true;
	while (loop)
	{
		loop = false;
		for (int collision = 0; collision < m_numOfPoints; collision++)
		{
            RigidBody& A = *m_points[collision].contactID1;
            RigidBody& B = *m_points[collision].contactID2;

            // TODO: Change it so sotre circles... AABB... Polygons all separately.
            //       Makes it easier to call appropriate functions for collision detection and response.
            

			Circle* circle1 = m_points[collision].contactID1;
			Circle* circle2 = m_points[collision].contactID2;

			if (distance(circle1->GetPos(), circle2->GetPos()) 
				> circle1->GetRadius() + circle2->GetRadius())
			{
				// No collision.
				for (int n = collision; n < m_numOfPoints - 1; ++n)
				{
					m_points[n] = m_points[n + 1];
				}
				--m_numOfPoints;
				loop = true;
			}
		}
	}
}
