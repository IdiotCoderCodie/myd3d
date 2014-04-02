#include "RigidBody.h"
#include "../../Entities/Entity.h"

RigidBody::RigidBody(void) 
: m_parentEntity(0)
{
}


RigidBody::~RigidBody(void)
{
}

void RigidBody::CalculatePhysics(float dt)
{
    // TO DO: Runge Kutta.!
    glm::vec2 force(0.0f, -98.10f * m_mass);
    glm::vec2 accel = force / m_mass;
    m_newVelocity = m_velocity + (accel * dt);
    m_newPosition = m_position + (m_newVelocity * dt);
}

void RigidBody::Update()
{
    m_velocity = m_newVelocity;
    m_position = m_newPosition;
	m_parentEntity->SetPos(glm::vec3(m_newPosition, 0.0f));
}

