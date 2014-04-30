#pragma once
#include "../Scenes/Scene.h"
#include "ContactManifold.h"
#include "RigidBody\RigidBody.h"
#include "RigidBody\Circle.h"
#include "RigidBody\AABB.h"
#include "RigidBody\RBPolygon.h"

class PhysicsSystem
{
public:
    PhysicsSystem(void);
    ~PhysicsSystem(void);

    void Update(double time);

	Circle& AddCircle(Entity* entity, float radius, glm::vec2& velocity, float mass);
    void AddAABB(Entity* entity, glm::vec2& min, glm::vec2& max, glm::vec2& velocity, float mass);

private:
	void SimulationLoop(double time);
	void StaticCollisionDetection();
	bool StaticSphereCollisionDetection(Circle& circle1, Circle& circle2);
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse();
	void UpdateObjectPhysics();

private:
	float m_dt;
    Scene* m_parentScene;
	ContactManifold* m_manifold;
	std::vector<Circle> m_circles;
    std::vector<AABB>   m_aabbs;
};

