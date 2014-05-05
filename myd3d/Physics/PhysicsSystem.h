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

#include <mutex>

#include <AntTweakBar.h>

#define EPSILON 0.0001

class PhysicsSystem : public Thread
{
public:
    PhysicsSystem(void);
    ~PhysicsSystem(void);

    int run();

    void Update(double time);

	Circle& AddCircle(Entity* entity, float radius, const glm::vec2& velocity, float mass, 
                      float elast);
    AABB& AddAABB(Entity* entity, const glm::vec2& min, const glm::vec2& max, 
                  const glm::vec2& velocity, float mass, float elast);

    void LoadNewCircles();

    void LoadNewAABBs();

    const glm::vec2& GetGravity() { return glm::vec2(m_gravity.x, m_gravity.y) * m_gravityScale; }

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
    float                   m_dtModifier;
    int                     m_targetfps;
    int                     m_actualfps;
    Scene*                  m_parentScene;
	ContactManifold*        m_manifold;
    std::vector<Circle*>    m_circles;
    std::vector<Circle*>    m_circlesToAdd;
    std::mutex              m_circleToAddMutex;
    std::vector<AABB*>      m_aabbs;   
    std::vector<AABB*>      m_aabbsToAdd;
    std::mutex              m_aabbsToAddMutex;
    TwBar*                  m_tweakBar;
    float                   m_gravityScale;// = 9.81f * 5.0f; // Could put in struct tbh.
    glm::vec3               m_gravity;// = glm::vec3(0.0f, -1.0f, 0.0f); // vec3 for AntTw.
};
