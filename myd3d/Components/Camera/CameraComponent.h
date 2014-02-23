#pragma once
#include "../Component.h"

#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"

class CameraComponent : public Component
{
public:
    CameraComponent();
    virtual ~CameraComponent();

    void FamilyID(componentId_t& out) const; 
    
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjMatrix() const { return m_ProjMatrix; }

    void SetProjMatrix(const glm::mat4& newMat) { m_ProjMatrix = newMat; }

    virtual void Update(float timeElapsed);

private:
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjMatrix;
};

