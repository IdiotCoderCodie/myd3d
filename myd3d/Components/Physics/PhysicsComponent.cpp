#include "PhysicsComponent.h"
#include "../../Entities/Entity.h"

PhysicsComponent::PhysicsComponent()
    :   m_mass(1.0f),
        m_velocity(0.0f, 0.0f, 0.0f),
        m_acceleration(0.0f, 0.0f, 0.0f),
        m_angularVelocity(0.0f, 0.0f, 0.0f),
        m_angularAccel(0.0f, 0.0f, 0.0f)
{
}


PhysicsComponent::PhysicsComponent(float mass)
                                   : m_mass(mass),
                                     m_velocity(0.0f, 0.0f, 0.0f),
                                     m_acceleration(0.0f, 0.0f, 0.0f),
                                     m_angularVelocity(0.0f, 0.0f, 0.0f),
                                     m_angularAccel(0.0f, 0.0f, 0.0f)
{

}

PhysicsComponent::PhysicsComponent(float mass,
                                   const glm::vec3& velocity)
                                   : m_mass(mass),
                                     m_velocity(velocity),
                                     m_acceleration(0.0f, 0.0f, 0.0f),
                                     m_angularVelocity(0.0f, 0.0f, 0.0f),
                                     m_angularAccel(0.0f, 0.0f, 0.0f)
{

}

PhysicsComponent::PhysicsComponent(float mass,
                                   const glm::vec3& velocity,
                                   const glm::vec3& acceleration)
                                   : m_mass(mass),
                                     m_velocity(velocity),
                                     m_acceleration(acceleration),
                                     m_angularVelocity(0.0f, 0.0f, 0.0f),
                                     m_angularAccel(0.0f, 0.0f, 0.0f)
{
}

PhysicsComponent::PhysicsComponent(float mass,
                                   const glm::vec3& velocity,
                                   const glm::vec3& acceleration,
                                   const glm::vec3& angularVelocity)
                                   : m_mass(mass),
                                     m_velocity(velocity),
                                     m_acceleration(acceleration),
                                     m_angularVelocity(angularVelocity),
                                     m_angularAccel(0.0f, 0.0f, 0.0f)
{
}

PhysicsComponent::PhysicsComponent(float mass,
                                   const glm::vec3& velocity,
                                   const glm::vec3& acceleration,
                                   const glm::vec3& angularVelocity,
                                   const glm::vec3& angularAccel)
                                   : m_mass(mass),
                                     m_velocity(velocity),
                                     m_acceleration(acceleration),
                                     m_angularVelocity(angularVelocity),
                                     m_angularAccel(angularAccel)
{
}


PhysicsComponent::~PhysicsComponent(void)
{
}


void PhysicsComponent::FamilyID(componentId_t& out) const     
{ 
    out = "PhysicsComponent"; 
}

void PhysicsComponent::ComponentID(componentId_t& out) const  
{ 
    out = "BasicPhysicsComponent"; 
}


void PhysicsComponent::Update(float time)
{
    m_velocity += (m_acceleration * time);
    GetParent().MoveForward(m_velocity.z * time);
    GetParent().MoveRight(m_velocity.x * time);
    GetParent().MoveUp(m_velocity.y * time);

    m_angularVelocity += m_angularAccel * time;
    GetParent().RotateLocalX(m_angularVelocity.x * time);
    GetParent().RotateLocalY(m_angularVelocity.y * time);
    GetParent().RotateLocalZ(m_angularVelocity.z * time);
}
