#pragma once
#include "VisualComponent.h"
#include "../../Assets/Textures/Texture.h"
#include "../../Assets/Textures/Bitmap.h"

class VisualBitmapComponent : public VisualComponent
{
private:
    struct VertexType
    {
        glm::vec3 position;
        glm::vec2 uv;
    };

public:
    VisualBitmapComponent(D3D& d3d, ID3D11ShaderResourceView* srcTexture, int width, int height,
                          int screenWidth, int screenHeight);
    VisualBitmapComponent(D3D& d3d, ID3D11ShaderResourceView* srcTexture, 
                           ID3D11ShaderResourceView* stencilTexture, int width, int height, 
                           int screenWidth, int screenHeight);

    virtual ~VisualBitmapComponent(void);

    virtual void ComponentID(componentId_t& out) const;

    virtual void Update(float time);

    virtual void Draw(D3D& d3d);

    void SetMaterialColor(float r, float g, float b) { m_materialColor = glm::vec3(r, g, b); }
    void SetMaterialColor(glm::vec3& color) { m_materialColor = color; }

private:
    Bitmap m_bitmap;
    ID3D11ShaderResourceView* m_stencilTexture;
    glm::vec3 m_materialColor;
};

