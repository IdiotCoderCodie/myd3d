#pragma once
#include "../../glm/glm.hpp"
#include "../ContactManifold.h"
class Entity;

class Circle;
class RBPolygon;

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
    virtual void CollisionWithCircle(Circle* circle, ContactManifold* contactManifold) = 0;
    virtual void CollisionWithPolygon(RBPolygon* polygon, ContactManifold* contactManifold) = 0;

    void Update();

    virtual void CollisionResponseWithCircle(ManifoldPoint& point) = 0;
    virtual void CollisionResponseWithPolygon(ManifoldPoint& point) = 0;


    void SetPos(float x, float y)       { m_position = glm::vec2(x, y); }
    void SetVel(float x, float y)       { m_velocity = glm::vec2(x, y); }
    void SetNewVel(float x, float y)    { m_newVelocity = glm::vec2(x, y); }
    void SetMass(float mass)            { m_mass = mass; }
    void SetElasticity(float elasticity){ m_elasticity = elasticity; }

    glm::vec2 GetPos() const            { return m_position; }
    glm::vec2 GetNewPos() const         { return m_newPosition; }
    glm::vec2 GetVel() const            { return m_velocity; }
    glm::vec2 GetNewVel() const         { return m_newVelocity; }
    float GetMass() const               { return m_mass; }
    float GetElasticity() const         { return m_elasticity; }

    void ResetPos()                     { m_newPosition = m_position; }

	void SetParent(Entity* entity)		{ m_parentEntity = entity; }

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
    int         m_objectID;
	Entity*     m_parentEntity;

    static int countId;
};

