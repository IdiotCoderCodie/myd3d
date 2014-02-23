#include "LightComponent.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/constants.hpp"
#include "../../Entities/Entity.h"

LightComponent::LightComponent(void)
    : m_active(true),
      m_ambient(0.0f, 0.0f, 0.0f, 1.0f), 
      m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
      m_specular(1.0f, 1.0f, 1.0f, 1.0f),
      m_spotCutoff(180.0f),
      m_spotExponent(0.0f),
      m_viewMatrix(1.0f),
      m_projectionMatrix(1.0f),
      m_tweakBarSetup(false)
{
}

LightComponent::LightComponent(const glm::vec4& ambient, const glm::vec4& diffuse, 
                               const glm::vec4& specular)
    : m_active(true),
      m_ambient(ambient), 
      m_diffuse(diffuse),
      m_specular(specular),    
      m_spotCutoff(180.0f),
      m_spotExponent(0.0f),
      m_viewMatrix(1.0f),
      m_projectionMatrix(1.0f),
      m_tweakBarSetup(false)
{
}


LightComponent::LightComponent(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular,
                               float spotCutoff, float spotExponent)
    : m_active(true),
      m_ambient(ambient),
      m_diffuse(diffuse),
      m_specular(specular),
      m_spotCutoff(spotCutoff),
      m_spotExponent(spotExponent),
      m_tweakBarSetup(false)
{
}



LightComponent::~LightComponent(void)
{
}


void LightComponent::FamilyID(componentId_t& out) const     
{ 
    out = "LightComponent"; 
}

void LightComponent::ComponentID(componentId_t& out) const  
{ 
    out = "SpotLightComponent"; 
} 


void LightComponent::GenerateProjectionMatrix(float near, float far)
{
    float fieldOfView  = 90.0f;
    float screenAspect = 1.0f;

    glm::mat4 ogl_to_d3d = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
    ogl_to_d3d = glm::scale(ogl_to_d3d, glm::vec3(1.0f, 1.0f, 0.5f));

    m_projectionMatrix = ogl_to_d3d * glm::perspective(fieldOfView, screenAspect, near, far);
}



void LightComponent::Update(float time)
{
    TweakBarSetup();

    Component::Update(time);
    
    const Frame& transform = GetParent().GetTransform();
    m_viewMatrix = glm::lookAt(transform.GetPosition(),
                               transform.GetPosition() + transform.GetForward(),
                               transform.GetUp());
}


bool LightComponent::TweakBarSetup()
{
    if(m_tweakBarSetup)
    {
        return true;
    }

    TwBar* tweakBar = GetParent().GetTweakBar();
    if(!tweakBar)
    {
        return false;
    }

    TwAddVarRW(tweakBar, "Ambient", TW_TYPE_COLOR4F, &m_ambient, "group='LightComponent' ");
    TwAddVarRW(tweakBar, "Diffuse", TW_TYPE_COLOR4F, &m_diffuse, "group='LightComponent' ");
    TwAddVarRW(tweakBar, "Specular", TW_TYPE_COLOR4F, &m_specular, "group='LightComponent' ");
    TwAddVarRW(tweakBar, "Spot Cutoff", TW_TYPE_FLOAT, &m_spotCutoff, 
               "group='LightComponent' step=0.01 ");
    TwAddVarRW(tweakBar, "Spot Exponent", TW_TYPE_FLOAT, &m_spotExponent, 
               "group='LightComponent' min=0 max=128");

    m_tweakBarSetup = true;
    return true;
}