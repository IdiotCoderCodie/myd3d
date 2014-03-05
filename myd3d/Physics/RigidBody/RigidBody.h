#pragma once
#include "../../glm/glm.hpp"
#include "../ContactManifold.h"

class RigidBody
{
public:
    RigidBody(void);
    virtual ~RigidBody(void);

    virtual void CalculatePhysics(float dt);
    virtual void CollisionWithCircle(RigidBody* circle, ContactManifold* contactManifold) = 0;
    virtual void CollisionWithSquare(RigidBody* square, ContactManifold* contactManifold) = 0;
    void Update();
    virtual void CollisionResponseWithCircle(ManifoldPoint& point) = 0;
    virtual void CollisionResponseWithSquare(ManifoldPoint& point) = 0;


    void SetPos(float x, float y)       { m_velocity = glm::vec2(x, y); }
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

private:
    float m_mass;
    float m_elasticity;
    glm::vec2 m_position;
    glm::vec2 m_newPosition;
    glm::vec2 m_velocity;
    glm::vec2 m_newVelocity;
    int m_objectID;

    static int countId;
};

