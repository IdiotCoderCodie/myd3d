#pragma once
#include "PhysicsComponent.h"
#include "../../InputManager.h"

//extern InputManager G_InputManager;

class FreeRoamFpComponent : public PhysicsComponent
{
public:
    FreeRoamFpComponent();
    FreeRoamFpComponent(float moveAccel, float turnSensitivityX, float turnSensitivityY);
    ~FreeRoamFpComponent(void);

    virtual void ComponentID(componentId_t& out) const; 

    virtual void Update(float time);

    float GetTurnSensitivityX() const { return m_turnSensitivityX; }
    float GetTurnSensitivityY() const { return m_turnSensitivityY; }

    float GetMoveAcceleration() const { return m_moveAcceleration; }
    float GetMoveDeceleration() const { return m_moveDeceleration; }

private:
    float m_turnSensitivityX; // Determine the ratio between how much mouse moves and how much the
    float m_turnSensitivityY; // object is turned.
    float m_moveAcceleration; // Acceleration of object when appropriate key is pressed.
    float m_moveDeceleration; // Deceleration of object when move key isn't pressed.
    
    /**
     * Below bools are used to allow smooth acceleration and deceleration of the camera.
     * Need these to ensure, when forward button is stopped need to begin to decelerate until 
     * stopped
     * and not go in reverse. However also need to not interfere with the backwards keypress.
     */
    bool  m_goingForward;
    bool m_goingBackward;
    bool m_goingRight;
    bool m_goingLeft;
    bool m_goingUp;
    bool m_goingDown;
};
