#include "VisualBitmapComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../../glm/gtc/matrix_transform.hpp"

VisualBitmapComponent::VisualBitmapComponent(D3D& d3d, ID3D11ShaderResourceView* srcTexture, 
                                             int width, int height, 
                                             int screenWidth, int screenHeight)
    :	VisualComponent(),
        m_bitmap(d3d, &(*srcTexture), width, height, screenWidth, screenHeight)
{
    if(!G_ShaderManager().IsLoaded())
    {
        G_ShaderManager().LoadShaders(d3d, "placeholder");
    }

    SetShader(G_ShaderManager().GetShader("Bitmap"));
}


VisualBitmapComponent::~VisualBitmapComponent(void)
{

}


void VisualBitmapComponent::ComponentID(componentId_t& out) const 
{ 
    out = "VisualBitmapComponent"; 
}


void VisualBitmapComponent::Update(float time)
{
    // Check if updating position. If so need to update bitmap buffers.
    m_bitmap.UpdateBuffers(GetParent().GetParent().GetParent().GetD3DInstance(), 
                           (int)GetParent().GetPos().x, (int)GetParent().GetPos().y);
}

void VisualBitmapComponent::Draw(D3D& d3d)
{
    d3d.TurnZBufferOff();
    // Bind bitmap stuff to the pipeline.
    m_bitmap.Draw(d3d);

    int screenWidth = GetParent().GetParent().GetParent().GetD3DInstance().GetScreenWidth();
    int screenHeight = GetParent().GetParent().GetParent().GetD3DInstance().GetScreenHeight();

    // NOTE: need to get the actual screen width and height.
    bitmap::MatrixBufferStruct matBuffer = 
    { 
        // Method of generating ortho matrix taken from D3DX spec.
        glm::mat4(2.0f / screenWidth, 0.0f, 0.0f, 0.0f,
                  0.0f, 2.0f / screenHeight, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f/(100.0f - 0.01f), 0.0f,
                  0.0f, 0.0f, 0.01f/(0.01f - 100.0f), 1.0f)     
    };

    GetShader().VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);

    ID3D11ShaderResourceView* tex = m_bitmap.GetTextureShaderResourceView();
    d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);

    GetShader().RenderShader(d3d, 6);
    d3d.TurnZBufferOn();
}