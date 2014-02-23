#pragma once
#include "CameraComponent.h"

class PerspectiveCamComponent : public CameraComponent
{
public:
    PerspectiveCamComponent(void);

    PerspectiveCamComponent(float fov, float aspect, float near, float far);

    ~PerspectiveCamComponent(void);

    void ComponentID(componentId_t& out) const;

private:
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
};
