#pragma once
#include "../Scenes/Scene.h"
#include "ContactManifold.h"
#include "RigidBody\RigidBody.h"
#include "RigidBody\Circle.h"

class PhysicsSystem
{
public:
    PhysicsSystem(void);
    ~PhysicsSystem(void);

    void Update(double time);

	void AddCircle(Entity* entity, float radius, glm::vec2& velocity);

private:
	void SimulationLoop(double time);
	void StaticCollisionDetection();
	bool StaticSphereCollisionDetection(Circle* circle1, Circle* circle2);
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse();
	void UpdateObjectPhysics();

private:
	float m_dt;
    Scene* m_parentScene;
	ContactManifold* m_manifold;
	std::vector<Circle*> m_circles;
};

