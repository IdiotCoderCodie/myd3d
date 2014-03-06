#include "PhysicsSystem.h"


PhysicsSystem::PhysicsSystem(void)
{
	m_manifold = new ContactManifold();
}


PhysicsSystem::~PhysicsSystem(void)
{
	for (auto c : m_circles)
	{
		delete c;
		c = 0;
	}
	m_circles.clear();

	delete m_manifold;
	m_manifold = 0;
	
	m_parentScene = 0;
}


void PhysicsSystem::Update(double time)
{
	// TODO: DO IT.
}


void PhysicsSystem::SimulationLoop(double time)
{
	// TODO: DO IT.
}


void PhysicsSystem::StaticCollisionDetection()
{
	// TODO: DO IT.
}


bool PhysicsSystem::StaticSphereCollisionDetection(Circle* circle1, Circle* circle2)
{
	// TODO: DO IT.
}


void PhysicsSystem::CalculateObjectPhysics()
{
	// TODO: DO IT.
}


void PhysicsSystem::DynamicCollisionDetection()
{
	// TODO: DO IT.
}


void PhysicsSystem::DynamicCollisionResponse()
{
	// TODO: DO IT.
}


void PhysicsSystem::UpdateObjectPhysics()
{
	// TODO: DO IT.
}