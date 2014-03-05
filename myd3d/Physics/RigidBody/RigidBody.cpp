#include "RigidBody.h"


RigidBody::RigidBody(void)
{
}


RigidBody::~RigidBody(void)
{
}

void RigidBody::CalculatePhysics(float dt)
{
    // TO DO: Runge Kutta.!
    glm::vec2 force(0.0f, -9.81f * m_mass);
    glm::vec2 accel = force / m_mass;
    m_newVelocity = m_velocity + (accel * dt);
    m_newPosition = m_position + (m_newVelocity * dt);
}

void RigidBody::Update()
{
    m_velocity = m_newVelocity;
    m_position = m_newPosition;
}

