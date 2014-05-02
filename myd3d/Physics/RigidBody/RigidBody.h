#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#pragma once
#include "../../glm/glm.hpp"
#include "../ContactManifold.h"
class Entity;

class Circle;
class RBPolygon;
class AABB;

#define RB_TYPE_CIRC 0
#define RB_TYPE_AABB 1
#define RB_TYPE_POLY 2

class RigidBody
{
    struct State
    {
        glm::vec2 x; // Position
        glm::vec2 v; // Velocity
    };

    struct Derivative
    {
        glm::vec2 dx; // Derivate of Position: Velocity
        glm::vec2 dv; // Derivative of Velocity: Acceleration
    };

public:
    RigidBody(void);
    virtual ~RigidBody(void);

    virtual void CalculatePhysics(float dt);
    virtual void CollisionWithCircle(Circle& circle, ContactManifold& contactManifold) = 0;
    virtual void CollisionWithAABB(AABB& aabb, ContactManifold& contactManifold) = 0;
    virtual void CollisionWithPolygon(RBPolygon& polygon, ContactManifold& contactManifold) = 0;

    void Update();

    virtual void CollisionResponse(ManifoldPoint& point) = 0;
   /* virtual void CollisionResponseWithCircle(ManifoldPoint& point) = 0;
    virtual void CollisionResponseWithAABB(ManifoldPoint& point) = 0;
    virtual void CollisionResponseWithPolygon(ManifoldPoint& point) = 0;*/

    //virtual int GetType() = 0;

    void SetPos(float x, float y)       { m_position = glm::vec2(x, y); }
    void SetPos(glm::vec2& pos)         { m_position = pos; }
    void SetNewPos(glm::vec2& pos)      { m_newPosition = pos; }
    void SetVel(float x, float y)       { m_velocity = glm::vec2(x, y); }
    void SetVel(const glm::vec2& vel)         { m_velocity = vel; }
    void SetNewVel(float x, float y)    { m_newVelocity = glm::vec2(x, y); }
    void SetNewVel(glm::vec2& vel)      { m_newVelocity = vel; }
    void SetMass(float mass)            { m_mass = mass; }
    void SetElasticity(float elasticity){ m_elasticity = elasticity; }
    void SetStaticFriction(float fric)  { m_staticFriction = fric; }
    void SetDynamicFriction(float fric) { m_dynamicFriction = fric; }

    glm::vec2 GetPos() const            { return m_position; }
    glm::vec2 GetNewPos() const         { return m_newPosition; }
    glm::vec2 GetVel() const            { return m_velocity; }
    glm::vec2 GetNewVel() const         { return m_newVelocity; }
    float GetMass() const               { return m_mass; }
    float GetInvMass() const            { return (m_mass == 0.0f ? 0.0f : 1.0f / m_mass); }
    float GetElasticity() const         { return m_elasticity; }
    float GetStaticFriction() const     { return m_staticFriction; }
    float GetDynamicFriction() const    { return m_dynamicFriction; }

    void ResetPos()                     { m_newPosition = m_position; }

	void SetParent(Entity* entity)		{ m_parentEntity = entity; }

    void ApplyForce(glm::vec2& force)   { m_force += force; }

    // RK4 stuff.
    Derivative Evaluate(const State& initial, float dt, const Derivative& d);
    glm::vec2 Acceleration(const State& state, float t);
    void Integrate(State& state,float dt);

private:
    float       m_mass;
    float       m_elasticity;
    glm::vec2   m_position;
    glm::vec2   m_newPosition;
    glm::vec2   m_velocity;
    glm::vec2   m_newVelocity;
    glm::vec2   m_force;
    float       m_staticFriction;
    float       m_dynamicFriction;
    int         m_objectID;
	Entity*     m_parentEntity;

    static int countId;
};

