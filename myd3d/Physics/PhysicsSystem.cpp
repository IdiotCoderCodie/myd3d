#include "PhysicsSystem.h"


PhysicsSystem::PhysicsSystem(void)
{
	m_manifold = new ContactManifold();
}


PhysicsSystem::~PhysicsSystem(void)
{
	m_circles.clear();

	delete m_manifold;
	m_manifold = 0;
	
	m_parentScene = 0;
}


void PhysicsSystem::Update(double time)
{
	// TODO: DO IT.
	SimulationLoop(time);
}


void PhysicsSystem::AddCircle(Entity* entity, float radius, glm::vec2& velocity)
{
    Circle circle;
	circle.SetParent(entity);
	circle.SetRadius(radius);
	circle.SetVel(velocity.x, velocity.y);
	circle.SetMass(1.0f);
	circle.SetPos(entity->GetPos().x, entity->GetPos().y);
    circle.SetElasticity(0.95f);

	m_circles.push_back(circle);
}

void PhysicsSystem::SimulationLoop(double time)
{
	m_dt = time;
	// TODO: DO IT.
	// Handle Static collisions.
	//StaticCollisionDetection();

	// Calculate the physics calculations on all objects.
	CalculateObjectPhysics();

	// Assess and remove any collisions that no longer take place.
	//m_manifold->Assess();
    m_manifold->Reset();

	// Handle dynamic collisions and update contact manifold for all objects.
	DynamicCollisionDetection();

	// Handle dynamic collision responses using the contact manifold for all objects.
	DynamicCollisionResponse();

	// Update the physics calculations on all objects (e.g. new positions, velocity, etc.)
	UpdateObjectPhysics();
}


void PhysicsSystem::StaticCollisionDetection()
{
	bool loop = true;
	// Do until there are no more static collisions.
	while (loop)
	{
		int checkedNum = 0; // used so we don't check same 2 spheres.
		loop = false;
		for (int circle1 = 0; circle1 < m_circles.size(); circle1++)
		{
			for (int circle2 = checkedNum + 1; circle2 < m_circles.size(); circle2++)
			{
				loop |= StaticSphereCollisionDetection(m_circles[circle1], m_circles[circle2]);
			}
			checkedNum++;
		}
	}
}


bool PhysicsSystem::StaticSphereCollisionDetection(Circle& circle1, Circle& circle2)
{
	// Check if the circles are intersecting, if they are, move them out a little.
	bool loop = false;
	glm::vec2 pos1 = circle1.GetPos();
	glm::vec2 pos2 = circle2.GetPos();

	float dist = glm::distance(pos1, pos2) - (circle1.GetRadius() + circle2.GetRadius());
	if (dist < 0.0f)
	{
		glm::vec2 colNormal = glm::normalize(pos1 - pos2);

		pos1 = pos1 - (colNormal * dist * 1.1f);
		circle1.SetPos(pos1.x, pos1.y);

		pos2 = pos2 + (colNormal * dist * 1.1f);
		circle2.SetPos(pos2.x, pos2.y);

		loop = true;
	}

	return loop;
}


void PhysicsSystem::CalculateObjectPhysics()
{
	for (auto& circle : m_circles)
	{
		circle.CalculatePhysics(m_dt);
	}
}


void PhysicsSystem::DynamicCollisionDetection()
{
	int checkedNum = 0; // used so we don't check same 2 spheres.
	
	for (int circle1 = 0; circle1 < m_circles.size(); circle1++)
	{
		for (int circle2 = circle1 + 1; circle2 < m_circles.size(); circle2++)
		{
			m_circles[circle1].CollisionWithCircle(m_circles[circle2], *m_manifold);
		}
		checkedNum++;
	}
}


void PhysicsSystem::DynamicCollisionResponse()
{
	for (int collision = 0; collision < m_manifold->GetNumPoints(); ++collision)
	{
		ManifoldPoint& point = m_manifold->GetPoint(collision);
		if (!point.responded)
		{
			point.contactID1->CollisionResponse(point);
		}
	}
}


void PhysicsSystem::UpdateObjectPhysics()
{
	for (auto& circle : m_circles)
	{
		circle.Update();
	}
}
