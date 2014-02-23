#pragma once
#include "../Component.h"

#include "../../glm/glm.hpp"

struct LightBufferStruct
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float specularPow;

};

class LightComponent : public Component
{
public:
    LightComponent(void);
    LightComponent(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular);
    LightComponent(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular,
                   float spotCutoff, float spotExponent);
    virtual ~LightComponent(void);

    void FamilyID(componentId_t& out) const;
    void ComponentID(componentId_t& out) const;

    const glm::vec4& GetAmbient() const                     { return m_ambient; }
    void SetAmbient(const glm::vec4& ambient)                { m_ambient = ambient; }
    void SetAmbient(float r, float g, float b, float a)     { m_ambient = glm::vec4(r, g, b, a); }

    const glm::vec4& GetDiffuse() const                     { return m_diffuse; }
    void SetDiffuse(const glm::vec4& diff)                  { m_diffuse = diff; }
    void SetDiffuse(float r, float g, float b, float a)     { m_diffuse = glm::vec4(r, g, b, a); }

    const glm::vec4& GetSpecular() const                    { return m_specular; }
    void SetSpecular(const glm::vec4& spec)                 { m_specular = spec; }
    void SetSpecular(float r, float g, float b, float a)    { m_specular = glm::vec4(r, g, b, a); }

    bool IsActive() const   { return m_active; }
    void Enable()           { m_active = true; }
    bool Disable()          { m_active = false; }

    float GetSpotCutoff() const { return m_spotCutoff; }
    void SetSpotCutoff(float cutoff) {  m_spotCutoff = cutoff; }

    float GetSpotExponent() const { return m_spotExponent; }
    void SetSpotExponent(float exp) { m_spotExponent = exp; }

    // Functions for shadow mapping.

    void GenerateProjectionMatrix(float near, float far);

    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& GetProjMatrix() const { return m_projectionMatrix; }

    virtual void Update(float time);
    
    bool TweakBarSetup();

private:
    bool m_active;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    float     m_spotCutoff;
    float     m_spotExponent;
    // Used for shadow mapping.
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    bool      m_tweakBarSetup;
};

