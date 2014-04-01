#include "VisualRaymarchComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../Assets/Shaders/ShaderManager.h"
#include "../../Assets/Textures/TextureManager.h"

#include "../../glm/gtc/matrix_transform.hpp"

//extern ShaderManager G_ShaderManager();
//extern TextureManager G_TextureManager;

static float   m_totalTime = 0.0f;

VisualRaymarchComponent::VisualRaymarchComponent(D3D& d3d, const std::string& filename, int raymarchId)
    : VisualComponent(),
      m_mesh(filename, d3d, false),
      m_raymarchId(raymarchId)
{

}



VisualRaymarchComponent::~VisualRaymarchComponent(void)
{
}


VisualRaymarchComponent& VisualRaymarchComponent::operator=(const VisualRaymarchComponent& other)
{
    m_mesh = other.m_mesh;
    return *this;
}


void VisualRaymarchComponent::ComponentID(componentId_t& out) const
{ 
    out = "VisualMeshComponent"; 
}


void VisualRaymarchComponent::Update(float timeElapsed)
{
    m_totalTime += timeElapsed;
}


void VisualRaymarchComponent::ShadowPass(D3D& d3d)
{
    //if(m_castShadows)
    //{
    //    m_mesh.Render(d3d, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //    Shader* shadowShader = G_ShaderManager().GetShader("Depth");
    //    // Send data for the matrix buffer to the shader, getting view and proj matrix from light.

    //    const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
    //    const auto& renderTargets = GetParent().GetParent().GetShadowMaps();
    //    // Get the first (1) lights and render their depth buffers.
    //    using namespace ConstantBuffers;
    //    for(int i = 0; i < glm::min(MAX_SHADOWCASTING_LIGHTS, (int)renderTargets.size()); i++)
    //    {
    //        renderTargets[i]->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());
    //        d3d.ClearDepthStencilView();

    //        ConstantBuffers::MVPBuffer matBuffer;
    //        matBuffer.modelMatrix = glm::transpose( GetParent().GetTransform().GetMatrix());
    //        // Get light and send it's data.
    //        LightComponent* light = static_cast<LightComponent*>(lights[i]);
    //        matBuffer.viewMatrix = glm::transpose( light->GetViewMatrix());
    //        matBuffer.projectionMatrix = glm::transpose(light->GetProjMatrix());

    //        shadowShader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&matBuffer,
    //                                           sizeof(matBuffer), 0);
    //        shadowShader->RenderShader(d3d, m_mesh.GetIndexCount());
    //    }
    //}
}


void VisualRaymarchComponent::Draw(D3D& d3d)
{
    m_mesh.Render(d3d, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   // if(m_recieveShadows)
    {
        // Draw using the shadow map to cast any shadows on the mesh.
        DrawWithShadows(d3d);
    }
    //else
    //{
    //    // Draw using standard shader, no shadows.
    //    DrawNoShadows(d3d);
    //}   
}


void VisualRaymarchComponent::DrawNoShadows(D3D& d3d)
{
   
}


void VisualRaymarchComponent::DrawWithShadows(D3D& d3d)
{
	SetShader(G_ShaderManager().GetShader("Raymarch"));

	// CAMERA BUFFER
	ConstantBuffers::RayMarchCameraBuffer cameraBuffer;
	cameraBuffer.eyePos = GetParent().GetParent().GetActiveCamera()->GetParent().GetPos();
	cameraBuffer.nearPlane = 1.0f;
	cameraBuffer.farPlane = 1000.0f;
	glm::mat4 inverse = GetParent().GetParent().GetActiveCamera()->GetViewMatrix();
	cameraBuffer.viewInverse =/* glm::transpose( */
		inverse  ;
	cameraBuffer.viewportH = d3d.GetScreenHeight();
	cameraBuffer.viewportW = d3d.GetScreenWidth();
    cameraBuffer.raymarchId = m_raymarchId;

	GetShader().PSSetConstBufferData(d3d, std::string("RaymarchCameraBuffer"), (void*)&cameraBuffer,
		sizeof(cameraBuffer), 0);


	// LIGHT BUFFER
	const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
	LightComponent* light = static_cast<LightComponent*>(lights[0]);

	ConstantBuffers::RayMarchLightBuffer lightBuffer;
	lightBuffer.lightColor = light->GetDiffuse();
	lightBuffer.lightPosition = light->GetParent().GetPos();

	GetShader().PSSetConstBufferData(d3d, std::string("RaymarchLightBuffer"), (void*)&lightBuffer,
		sizeof(lightBuffer), 1);


	// BACKGROUND BUFFER
	ConstantBuffers::RayMarchBackgroundColorBuffer backgroundBuffer;
	backgroundBuffer.backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

	GetShader().PSSetConstBufferData(d3d, std::string("RaymarchBackgroundColorBuffer"),
		(void*)&backgroundBuffer, sizeof(backgroundBuffer), 2);


    // Render with shader.
    GetShader().RenderShader(d3d, m_mesh.GetIndexCount());
}
