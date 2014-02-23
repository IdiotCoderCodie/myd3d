#pragma once
#include "../Component.h"
#include "../../Assets/Shaders/Shader.h"
#include "../../Assets/Shaders/ShaderManager.h"


class VisualComponent : public Component
{
public:
    VisualComponent();

    virtual ~VisualComponent();

    virtual void FamilyID(componentId_t& out) const;

    //virtual void Draw(D3D& d3d);

    void SetShader(Shader* shader) { m_Shader = &*(shader); }
    const Shader& GetShader() const { return *m_Shader; }
    Shader& GetShader() { return *m_Shader; }
    //Shader& GetShader() { return *m_Shader; }

private:
    Shader* m_Shader;
};
