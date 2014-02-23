#include "FreeRoamFpComponent.h"

static const float FLOAT_ZERO_DELTA = 0.00001f;
//#define CHECK_FLOAT_ZERO(X) ((X > -FLOAT_ZERO_DELTA) && (X < FLOAT_ZERO_DELTA))

FreeRoamFpComponent::FreeRoamFpComponent(void)
    : PhysicsComponent(),
      m_turnSensitivityX(100.0f), 
      m_turnSensitivityY(10.0f),
      m_moveAcceleration(0.1f), 
      m_moveDeceleration(0.1f),
      m_goingForward(false), 
      m_goingBackward(false), 
      m_goingRight(false),
      m_goingLeft(false),
      m_goingUp(false), 
      m_goingDown(false)
      
{
}


FreeRoamFpComponent::FreeRoamFpComponent(float moveAccel, float turnSensitivityX, 
                                         float turnSensitivityY)
    : PhysicsComponent(),
     m_turnSensitivityX(turnSensitivityX),
     m_turnSensitivityY(turnSensitivityY),
     m_moveAcceleration(moveAccel),
     m_moveDeceleration(moveAccel * 3.0f),
     m_goingForward(false), 
     m_goingBackward(false), 
     m_goingRight(false),
     m_goingLeft(false),
     m_goingUp(false), 
     m_goingDown(false)
{

}


FreeRoamFpComponent::~FreeRoamFpComponent(void)
{
}


void FreeRoamFpComponent::ComponentID(componentId_t& out) const  
{ 
    out = "FreeRoamFpPhysicsComponent"; 
}


void FreeRoamFpComponent::Update(float time)
{
    glm::vec3 newAccel = glm::vec3(0.0f);

    //----------------------------------------------------------------------------------------------
    // Handling of z axis movement.
    //----------------------------------------------------------------------------------------------

    // Handle forwards keypress.
    if(G_InputManager().IsKeyPressed(DIK_UPARROW) && G_InputManager().IsKeyPressed(DIK_LCONTROL) 
        && !m_goingBackward)
    {
        // Set z acceleration.
        newAccel.z += m_moveAcceleration;
        m_goingForward = true;
        m_goingBackward = false;
    }
    else if(GetVelocity().z < 0.00001f && m_goingForward)
    { 
        // Stop it from starting to go backwards.
        newAccel.z = 0.0f;
        SetVelocity(GetVelocity().x, GetVelocity().y, 0.0f);
        m_goingForward = false;
    }
    else if (m_goingForward)
    {
        // Carry on / start decelerating.
        newAccel.z = -m_moveDeceleration;
    }

    // Handle backwards key press.
    if(G_InputManager().IsKeyPressed(DIK_DOWNARROW) && G_InputManager().IsKeyPressed(DIK_LCONTROL)
        && !m_goingForward)
    {
        // Set z acceleration.
        newAccel.z -= m_moveAcceleration;
        m_goingForward = false;
        m_goingBackward = true;
    }
    else if(GetVelocity().z > -0.00001f && m_goingBackward)
    { 
        // Stop it from starting to go backwards.
        newAccel.z = 0.0f;
        SetVelocity(GetVelocity().x, GetVelocity().y, 0.0f);
        m_goingBackward = false;
    }
    else if (m_goingBackward)
    {
        // Carry on / start decelerating.
        newAccel.z = +m_moveDeceleration;
    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Handling of x axis movement.
    //----------------------------------------------------------------------------------------------

     // Handle right key press.
    if(G_InputManager().IsKeyPressed(DIK_RIGHTARROW) && G_InputManager().IsKeyPressed(DIK_LCONTROL) 
        && !m_goingLeft)
    {
        // Set x acceleration.
        newAccel.x += m_moveAcceleration;
       
        m_goingRight = true;
        m_goingLeft = false;
    }
    else if(GetVelocity().x < 0.00001f && m_goingRight)
    { 
        // Stop it from starting to go backwards.
        newAccel.x = 0.0f;
        SetVelocity(0.0f, GetVelocity().y, GetVelocity().z);
        m_goingRight = false;
    }
    else if (m_goingRight)
    {
        // Carry on / start decelerating.
        newAccel.x = -m_moveDeceleration;
    }


    // Handle left keypress.
    if(G_InputManager().IsKeyPressed(DIK_LEFTARROW) && G_InputManager().IsKeyPressed(DIK_LCONTROL) 
        && !m_goingRight)
    {
        // Set x acceleration.
        newAccel.x -= m_moveAcceleration;
        m_goingLeft = true;
        m_goingRight = false;
    }
    else if(GetVelocity().x > -0.00001f && m_goingLeft)
    { 
        // Stop it from starting to go backwards.
        newAccel.x = 0.0f;
        SetVelocity(0.0f, GetVelocity().y, GetVelocity().z);
        m_goingLeft = false;
    }
    else if (m_goingLeft)
    {
        // Carry on / start decelerating.
        newAccel.x = +m_moveDeceleration;
    }  
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Handling moving up Y axis.
    //----------------------------------------------------------------------------------------------

     // Handle PGUP key press.
    if(G_InputManager().IsKeyPressed(DIK_PGUP) && G_InputManager().IsKeyPressed(DIK_LCONTROL) 
        && !m_goingDown)
    {
        // Set y acceleration.
        newAccel.y = +m_moveAcceleration;
       
        m_goingUp = true;
        m_goingDown = false;
    }
    else if(GetVelocity().y < 0.00001f && m_goingUp)
    { 
        // Stop it from starting to go backwards.
        newAccel.y = 0.0f;
        SetVelocity(GetVelocity().x, 0.0f, GetVelocity().z);
        m_goingUp = false;
    }
    else if (m_goingUp)
    {
        // Carry on / start decelerating.
        newAccel.y = -m_moveDeceleration;
    }


    // Handle left keypress.
    if(G_InputManager().IsKeyPressed(DIK_PGDN) && G_InputManager().IsKeyPressed(DIK_LCONTROL) 
        && !m_goingUp)
    {
        // Set y acceleration.
        newAccel.y = -m_moveAcceleration;
        m_goingDown = true;
        m_goingUp = false;
    }
    else if(GetVelocity().y > -0.00001f && m_goingDown)
    { 
        // Stop it from starting to go backwards.
        newAccel.y = 0.0f;
        SetVelocity(GetVelocity().x, 0.0f, GetVelocity().z);
        m_goingDown = false;
    }
    else if (m_goingDown)
    {
        // Carry on / start decelerating.
        newAccel.y = +m_moveDeceleration;
    }  
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Handle Rotation about Y axis (Left, Right).
    //----------------------------------------------------------------------------------------------
    glm::vec3 newAngularVelocity = glm::vec3(0.0f);

    if(G_InputManager().IsKeyPressed(DIK_LEFTARROW) && !G_InputManager().IsKeyPressed(DIK_LCONTROL))
    {
        newAngularVelocity.y += m_turnSensitivityX;
    }
    if(G_InputManager().IsKeyPressed(DIK_RIGHTARROW) && !G_InputManager().IsKeyPressed(DIK_LCONTROL))
    {
        newAngularVelocity.y -= m_turnSensitivityX;
    }
    if(G_InputManager().IsKeyPressed(DIK_UPARROW) && !G_InputManager().IsKeyPressed(DIK_LCONTROL))
    {
        newAngularVelocity.x -= m_turnSensitivityY;
    }
    if(G_InputManager().IsKeyPressed(DIK_DOWNARROW) && !G_InputManager().IsKeyPressed(DIK_LCONTROL))
    {
        newAngularVelocity.x += m_turnSensitivityY;
    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    SetAngularVelocity(newAngularVelocity);
    SetAccel(newAccel);

    PhysicsComponent::Update(time);
}
