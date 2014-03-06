#include "Circle.h"


Circle::Circle(void) : m_radius(1.0f)
{
}


Circle::~Circle(void)
{
}

void Circle::CollisionWithCircle(Circle* circle, ContactManifold* contactManifold)
{
    if((circle->GetNewPos() - this->GetNewPos()).length() < circle->GetRadius() + this->GetRadius())
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
    // TODO: complete.
	Circle* c1 = point.contactID1;
	Circle* c2 = point.contactID2;
	float m1 = c1->GetMass();
	float m2 = c2->GetMass();
	glm::vec2 u1 = c1->GetNewVel();
	glm::vec2 u2 = c2->GetNewVel();
	
	glm::vec2 v1 = ((m1 - m2) / (m1 + m2))*u1 + ((2 * m2) / (m1 + m2))*u2;
	glm::vec2 v2 = ((m2 - m1) / (m1 + m2))*u2 + ((2 * m1) / (m1 + m2))*u1;

	c1->SetNewVel(v1.x, v1.y);
	c2->SetNewVel(v2.x, v2.y);

	point.responded = true;
}

void Circle::CollisionResponseWithSquare(ManifoldPoint& point)
{
    // TODO: complete.
}
