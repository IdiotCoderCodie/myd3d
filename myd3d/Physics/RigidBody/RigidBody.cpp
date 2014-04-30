#include "RigidBody.h"
#include "../../Entities/Entity.h"

RigidBody::RigidBody(void) 
:   m_mass(1.0f),
    m_elasticity(1.0f),
    m_position(0.0f),
    m_newPosition(0.0f),
    m_velocity(0.0f),
    m_newVelocity(0.0f),
    m_force(0.0f),
    m_staticFriction(0.05f),
    m_dynamicFriction(0.02f),
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
	m_parentEntity->SetPos(glm::vec3(m_newPosition, 0.0f));

    //if(m_position.y < -200.0f)
    //{
    //    m_velocity.y = abs(m_velocity.y);
    //}
    //if(m_position.x < -300.0f)
    //{
    //    m_velocity.x = abs(m_velocity.x);
    //}
    //else if(m_position.x > 300.0f)
    //{
    //    m_velocity.x = -abs(m_velocity.x);
    //}
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
    // TODO: Calculate the acceleration from forces acting.
    /*glm::vec2 force(0.0f, -98.1f * m_mass);*/
    m_force += glm::vec2(0.0f, -98.10f * m_mass);
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
