#include "Circle.h"


Circle::Circle(void) : m_radius(1.0f)
{
}


Circle::~Circle(void)
{
}

void Circle::CollisionWithCircle(Circle* circle, ContactManifold* contactManifold)
{
	glm::vec2 test1 = circle->GetNewPos() - this->GetNewPos();

    if( glm::length(circle->GetNewPos() - this->GetNewPos()) < (circle->GetRadius() + this->GetRadius()) )
    {
        ManifoldPoint newPoint;
        newPoint.contactID1     = this;
        newPoint.contactID2     = circle;
        newPoint.contactNormal  = glm::normalize(this->GetNewPos() - circle->GetNewPos());
        newPoint.contactPoint   = circle->GetNewPos() + (circle->GetRadius() * newPoint.contactNormal);
        newPoint.responded      = false;
        contactManifold->Add(newPoint);
    }
}

void Circle::CollisionWithSquare(RigidBody* circle, ContactManifold* contactManifold)
{
    // TODO: complete.
}

void Circle::CollisionResponseWithCircle(ManifoldPoint& point)
{
    using namespace glm;
    // TODO: complete.
	Circle* c1 = point.contactID1;
	Circle* c2 = point.contactID2;
	float m1 = c1->GetMass();
	float m2 = c2->GetMass();
	glm::vec2 u1 = c1->GetNewVel();
	glm::vec2 u2 = c2->GetNewVel();

    glm::vec2 L = glm::normalize(point.contactNormal);

    glm::vec2 vl1 = ((m1 - m2) * (dot(u1, L)) * L + (m2 + m2) * (dot(u2, L)) * L) / (m1 + m2);
    glm::vec2 vl2 = ((m1 + m2) * dot(u1, L) * L + (m2 - m1) * dot(u2, L) * L) / (m1 + m2);

    glm::vec2 v1 = u1 - glm::dot(u1, L) * L + vl1;
    glm::vec2 v2 = u2 - dot(u2, L) * L + vl2;
	
	/*glm::vec2 v1 = ((m1 - m2) / (m1 + m2))*u1 + ((2 * m2) / (m1 + m2))*u2;
	glm::vec2 v2 = ((m2 - m1) / (m1 + m2))*u2 + ((2 * m1) / (m1 + m2))*u1;*/

	c1->SetNewVel(v1.x, v1.y);
	c2->SetNewVel(v2.x, v2.y);

	point.responded = true;
}

void Circle::CollisionResponseWithSquare(ManifoldPoint& point)
{
    // TODO: complete.
}
