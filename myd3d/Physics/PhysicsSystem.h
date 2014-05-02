#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#pragma once

#include "../Thread/Thread.h"
#include "../Scenes/Scene.h"
#include "ContactManifold.h"
#include "RigidBody\RigidBody.h"
#include "RigidBody\Circle.h"
#include "RigidBody\AABB.h"
#include "RigidBody\RBPolygon.h"
#include "../HiResTimer.h"

#include <AntTweakBar.h>

#define EPSILON 0.0001

class PhysicsSystem : public Thread
{
public:
    PhysicsSystem(void);
    ~PhysicsSystem(void);

    int run();

    void Update(double time);

	Circle& AddCircle(Entity* entity, float radius, const glm::vec2& velocity, float mass, float elast);
    AABB& AddAABB(Entity* entity, const glm::vec2& min, const glm::vec2& max, 
                 const glm::vec2& velocity, float mass, float elast);

private:
	void SimulationLoop(double time);
	void StaticCollisionDetection();
	bool StaticSphereCollisionDetection(Circle& circle1, Circle& circle2);
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse();
	void UpdateObjectPhysics();

private:
    HiResTimer              m_timer;
	float                   m_dt;
    int                     m_targetfps;
    Scene*                  m_parentScene;
	ContactManifold*        m_manifold;
	//std::vector<Circle> m_circles;
    std::vector<Circle*>    m_circles;
    std::vector<AABB*>      m_aabbs;   
    TwBar*                  m_tweakBar;
    float                   m_gravityScale  = 9.81f;
    glm::vec3               m_gravity       = glm::vec3(0.0f, -1.0f, 0.0f);
};
