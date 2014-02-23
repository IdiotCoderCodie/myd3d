#pragma once
#include "../Component.h"
#include "../../glm/glm.hpp"

class PhysicsComponent : public Component
{
public:
    PhysicsComponent();

    explicit PhysicsComponent(float mass);

    PhysicsComponent(float mass,
                     const glm::vec3& velocity);

    PhysicsComponent(float mass,
                     const glm::vec3& velocity,
                     const glm::vec3& acceleration);

    PhysicsComponent(float mass,
                     const glm::vec3& velocity,
                     const glm::vec3& acceleration,
                     const glm::vec3& angularVelocity);

    PhysicsComponent(float mass,
                     const glm::vec3& velocity,
                     const glm::vec3& acceleration,
                     const glm::vec3& angularVelocity,
                     const glm::vec3& angularAccel);

    virtual ~PhysicsComponent(void);


    void FamilyID(componentId_t& out) const;
    virtual void ComponentID(componentId_t& out) const;
    
    virtual void Update(float time);

    const glm::vec3& GetVelocity() const                { return m_velocity; } 
    const glm::vec3& GetAcceleration() const            { return m_acceleration; }
    const glm::vec3& GetAngularVelocity() const         { return m_angularVelocity; }
    const glm::vec3& GetAngularAccel() const            { return m_angularAccel; }
    float GetMass() const                               { return m_mass; }

    void SetVelocity(const glm::vec3& velocity)         { m_velocity = velocity; }
    void SetAccel(const glm::vec3& accel)               { m_acceleration = accel; }
    void SetAngularVelocity(const glm::vec3& angVel)    { m_angularVelocity = angVel; }
    void SetAngulatAccel(const glm::vec3& angAccel)     { m_angularAccel = angAccel; }

    void SetVelocity(float x, float y, float z)         { m_velocity = glm::vec3(x, y, z); }
    void SetAccel(float x, float y, float z)            { m_acceleration = glm::vec3(x, y, z); }
    void SetAngularVelocity(float x, float y, float z)  { m_angularVelocity = glm::vec3(x, y, z); }
    void SetAngularAccel(float x, float y, float z)     { m_angularAccel = glm::vec3(x, y, z); }
    void SetMass(float mass)                            { m_mass = mass; }

private:
    float m_mass;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;
    glm::vec3 m_angularVelocity;
    glm::vec3 m_angularAccel;
};

