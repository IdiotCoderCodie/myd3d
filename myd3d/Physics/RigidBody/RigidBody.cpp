#include "RigidBody.h"
#include "../../Entities/Entity.h"
#include <iostream>
#include "../../glm/gtx/norm.hpp" // length2

RigidBody::RigidBody(void) 
:   m_mass(1.0f),
    m_elasticity(1.0f),
    m_position(0.0f),
    m_newPosition(0.0f),
    m_velocity(0.0f),
    m_newVelocity(0.0f),
    m_force(0.0f),
    m_staticFriction(0.2f),
    m_dynamicFriction(0.05f),
    m_parentEntity(0)
{
}


RigidBody::~RigidBody(void)
{
}

void RigidBody::CalculatePhysics(float dt)
{
    // TO DO: Runge Kutta.!
    State newState;
    newState.x = m_position;
    newState.v = m_velocity;
    Integrate(newState, dt);
    m_newPosition = newState.x;
    m_newVelocity = newState.v;

    m_force = glm::vec2(0.0f); // Reset force for next update.
    /*glm::vec2 force(0.0f, -9.810f * m_mass);
    glm::vec2 accel = force / m_mass;
    m_newVelocity = m_velocity + (accel * dt);
    m_newPosition = m_position + (m_newVelocity * dt);*/
}

void RigidBody::Update()
{
    m_velocity = m_newVelocity;
    m_position = m_newPosition;
    if (m_parentEntity)
    {
       // m_parentEntity->SetPos(glm::vec3(m_newPosition, 0.0f));
    }
}


RigidBody::Derivative RigidBody::Evaluate(const State& initial, float dt, const Derivative& d)
{
    State state;
    state.x = initial.x + d.dx * dt;
    state.v = initial.v + d.dv * dt;

    Derivative output;
    output.dx = state.v;
    output.dv = Acceleration(state, dt);
    return output;
}

glm::vec2 RigidBody::Acceleration(const State& state, float dt)
{
    //glm::vec2 force = m_force * m_mass;
    glm::vec2 accel = m_force * GetInvMass();  // m_mass;

    return accel;
}

void RigidBody::Integrate(State& state, float dt)
{
    // Sample 4 derivatives.
    Derivative a = Evaluate(state, 0.0f, Derivative()); 
    Derivative b = Evaluate(state, dt * 0.5f, a);
    Derivative c = Evaluate(state, dt * 0.5f, b);
    Derivative d = Evaluate(state, dt, c);

    const glm::vec2 dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
    const glm::vec2 dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);

    state.x = state.x + dxdt * dt;
    state.v = state.v + dvdt * dt;
}

void RigidBody::CollisionResponse(ManifoldPoint& point, float time)
{
    using namespace glm;

    RigidBody& A = *point.contactID1;
    RigidBody& B = *point.contactID2;

    // Relative velocity.
    vec2 rv = B.GetNewVel() - A.GetNewVel();

    // Get in terms of normal direction.
    float velAlongNorm = glm::dot(rv, point.contactNormal);

    // Check if velocities are moving apart.
    if (velAlongNorm > 0.0f)
        return;

    float e = glm::min(A.GetElasticity(), B.GetElasticity());
    if (glm::length2(rv) < 0.00001)
        e = 0.0f;

    // Calc impulse scalar
    float j = -(1.0f + e) * velAlongNorm;
    j /= A.GetInvMass() + B.GetInvMass();

    // Apply impulse.
    vec2 impulse = j * point.contactNormal;
    A.SetNewVel(A.GetNewVel() - (A.GetInvMass()) * impulse);
    B.SetNewVel(B.GetNewVel() + (B.GetInvMass()) * impulse);

    point.responded = true;

    // Solve friction.
    // Get new relative velocity.
    vec2 rvT = B.GetNewVel() - A.GetNewVel();

    // solve for tangent vec.
    vec2 tangent = rvT - dot(rvT, point.contactNormal) * point.contactNormal;
    if (glm::length2(tangent) > 0.01)
    {
        tangent = glm::normalize(tangent);

        // Solve impulse magnitude along tangent.
        float jt = -dot(rvT, tangent);
        jt = jt / (A.GetInvMass() + B.GetInvMass());

        // Get friction component. of Ff = uFn
        float u = (A.GetStaticFriction() + B.GetStaticFriction()) / 2.0f; // sqrt(A.GetStaticFriction() * A.GetStaticFriction() + B.GetStaticFriction() * B.GetStaticFriction());

        // Clamp magnitude.
        vec2 frictionImpulse;
        if (abs(jt) < j * u)
        {
            frictionImpulse = jt * tangent;
        }
        else
        {
            float dynamicFriction = (A.GetDynamicFriction() + B.GetDynamicFriction()) / 2.0f;
            //float dynamicFriction = sqrt(A.GetDynamicFriction() * A.GetDynamicFriction() + B.GetDynamicFriction() * B.GetDynamicFriction());
            frictionImpulse = -j * tangent * dynamicFriction;
        }

        A.SetNewVel(A.GetNewVel() - A.GetInvMass() * frictionImpulse);
        B.SetNewVel(B.GetNewVel() + B.GetInvMass() * frictionImpulse);  
    }
    // Positional Correction.
    const float percent = 0.2;
    const float slop = 0.01f;

    glm::vec2 correction = glm::max(point.penetration - slop, 0.0f)
        / (A.GetInvMass() + B.GetInvMass()) * percent * point.contactNormal;

    A.SetNewPos(A.GetNewPos() - A.GetInvMass() * correction);
    B.SetNewPos(B.GetNewPos() + B.GetInvMass() * correction);
}