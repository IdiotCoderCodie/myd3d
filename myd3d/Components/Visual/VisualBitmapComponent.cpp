#include "VisualBitmapComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../Assets/Shaders/ShaderResources/constant_buffers.h"
#include "../Camera/CameraComponent.h"

VisualBitmapComponent::VisualBitmapComponent(D3D& d3d, ID3D11ShaderResourceView* srcTexture, 
                                             int width, int height, 
                                             int screenWidth, int screenHeight)
    :	VisualComponent(),
        m_bitmap(d3d, &(*srcTexture), width, height, screenWidth, screenHeight),
        m_stencilTexture(0)
{
    if(!G_ShaderManager().IsLoaded())
    {
        G_ShaderManager().LoadShaders(d3d, "placeholder");
    }

    SetShader(G_ShaderManager().GetShader("Bitmap"));
}

VisualBitmapComponent::VisualBitmapComponent(D3D& d3d, ID3D11ShaderResourceView* srcTexture, 
                                             ID3D11ShaderResourceView* stencilTexture,
                                             int width, int height, 
                                             int screenWidth, int screenHeight)
    :	VisualComponent(),
        m_bitmap(d3d, &(*srcTexture), width, height, screenWidth, screenHeight),
        m_stencilTexture(&(*stencilTexture))
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
}

void VisualBitmapComponent::Draw(D3D& d3d)
{
    d3d.TurnZBufferOff();
    // Bind bitmap stuff to the pipeline.
    m_bitmap.Draw(d3d);

    int screenWidth = GetParent().GetParent().GetParent().GetD3DInstance().GetScreenWidth();
    int screenHeight = GetParent().GetParent().GetParent().GetD3DInstance().GetScreenHeight();

	// TODO: Get the actual view matrix from the active camera.
	// TODO: Create an orthogonal camera component.

    if(!m_stencilTexture)
    {
        SetShader(G_ShaderManager().GetShader("Bitmap"));
    }
    else
    {
        SetShader(G_ShaderManager().GetShader("Bitmap_Stencil"));
    }

	ConstantBuffers::MVPBuffer matrixBuffer;
	matrixBuffer.modelMatrix = glm::transpose(GetParent().GetTransform().GetMatrix());
	matrixBuffer.viewMatrix	 = glm::transpose(glm::mat4(1.0f));
	matrixBuffer.projectionMatrix = glm::mat4(2.0f / screenWidth, 0.0f, 0.0f, 0.0f,
										      0.0f, 2.0f / screenHeight, 0.0f, 0.0f,
											  0.0f, 0.0f, 1.0f / (100.0f - 0.01f), 0.0f,
											  0.0f, 0.0f, 0.01f / (0.01f - 100.0f), 1.0f);

	matrixBuffer.viewMatrix = glm::transpose(GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
	matrixBuffer.projectionMatrix = glm::transpose(
		GetParent().GetParent().GetActiveCamera()->GetProjMatrix());
	/* THIS WORKS. BUT WON'T USE YET UNLESS NEEDED FOR SOME REASON.
	glm::mat4 ogl_to_d3d = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
	ogl_to_d3d = glm::scale(ogl_to_d3d, glm::vec3(1.0f, 1.0f, 0.5f));
	glm::mat4 test = glm::ortho(-screenWidth / 2.0f, screenWidth / 2.0f, -screenHeight / 2.0f, screenHeight / 2.0f,
								0.01f, 100.0f);
								
	test = ogl_to_d3d * test;

	matrixBuffer.projectionMatrix = test;
	*/

    GetShader().VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matrixBuffer, sizeof(matrixBuffer), 0);

    ID3D11ShaderResourceView* tex = m_bitmap.GetTextureShaderResourceView();
    d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);
    if(m_stencilTexture)
    {
        d3d.GetDeviceContext().PSSetShaderResources(1, 1, &m_stencilTexture);
    }

    GetShader().RenderShader(d3d, 6);
    d3d.TurnZBufferOn();
}