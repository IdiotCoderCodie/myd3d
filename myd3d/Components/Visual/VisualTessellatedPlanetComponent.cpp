#include "VisualTessellatedPlanetComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../Assets/Shaders/ShaderManager.h"
#include "../../Assets/Textures/TextureManager.h"

#include "../../glm/gtc/matrix_transform.hpp"

static float   m_totalTime = 0.0f;

VisualTessellatedPlanetComponent::VisualTessellatedPlanetComponent(D3D& d3d, const std::string& filename, Texture& texture,
                                         std::vector<RenderTarget*>& shadowMaps)
    : VisualComponent(),
      m_mesh(filename, d3d, false), 
	  m_texture(texture),
      m_bumpTexture(texture), // Not used anyway, so set as same as m_texture for now?
      m_shadowMaps(shadowMaps),
      m_castShadows(false),
      m_recieveShadows(false)
{
    if(!G_ShaderManager().IsLoaded())
    {
        G_ShaderManager().LoadShaders(d3d, "configFile");
    }
   SetShader(G_ShaderManager().GetShader("Normal_Shadows_Test"));
}


VisualTessellatedPlanetComponent::VisualTessellatedPlanetComponent(D3D& d3d, const std::string& filename, Texture& texture,
                                         Texture& bmpMap, std::vector<RenderTarget*>& shadowMaps)
    : VisualComponent(),
      m_mesh(filename, d3d, true),
      m_texture(texture),
      m_bumpTexture(bmpMap),
      m_shadowMaps(shadowMaps),
      m_castShadows(false),
      m_recieveShadows(false)
{
    if(!G_ShaderManager().IsLoaded())
    {
        G_ShaderManager().LoadShaders(d3d, "configFile");
    }
    SetShader(G_ShaderManager().GetShader("Normal_Shadows_Test"));
}


VisualTessellatedPlanetComponent::~VisualTessellatedPlanetComponent(void)
{
}


VisualTessellatedPlanetComponent& VisualTessellatedPlanetComponent::operator=(const VisualTessellatedPlanetComponent& other)
{
    m_mesh = other.m_mesh;
    m_texture;
    m_bumpTexture;
    m_shadowMaps;
    m_castShadows = other.m_castShadows;
    m_recieveShadows = other.m_recieveShadows;
    return *this;
}


void VisualTessellatedPlanetComponent::ComponentID(componentId_t& out) const
{ 
    out = "VisualTessellatedPlanetComponent"; 
}


void VisualTessellatedPlanetComponent::Update(float timeElapsed)
{
    m_totalTime += timeElapsed;
}


void VisualTessellatedPlanetComponent::ShadowPass(D3D& d3d)
{
    if(m_castShadows)
    {
        m_mesh.Render(d3d, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        Shader* shadowShader = G_ShaderManager().GetShader("Depth");
        // Send data for the matrix buffer to the shader, getting view and proj matrix from light.

        const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
        const auto& renderTargets = GetParent().GetParent().GetShadowMaps();
        // Get the first (1) lights and render their depth buffers.
        using namespace ConstantBuffers;
        for(int i = 0; i < glm::min(MAX_SHADOWCASTING_LIGHTS, (int)renderTargets.size()); i++)
        {
            renderTargets[i]->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());
            d3d.ClearDepthStencilView();

            ConstantBuffers::MVPBuffer matBuffer;
            matBuffer.modelMatrix = glm::transpose( GetParent().GetTransform().GetMatrix());
            // Get light and send it's data.
            LightComponent* light = static_cast<LightComponent*>(lights[i]);
            matBuffer.viewMatrix = glm::transpose( light->GetViewMatrix());
            matBuffer.projectionMatrix = glm::transpose(light->GetProjMatrix());

            shadowShader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&matBuffer,
                                               sizeof(matBuffer), 0);
            shadowShader->RenderShader(d3d, m_mesh.GetIndexCount());
        }
    }
}


void VisualTessellatedPlanetComponent::Draw(D3D& d3d)
{
    m_mesh.Render(d3d, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

    if(m_recieveShadows)
    {
        // Draw using the shadow map to cast any shadows on the mesh.
        DrawWithShadows(d3d);
    }
    else
    {
        // Draw using standard shader, no shadows.
        DrawNoShadows(d3d);
    }   
}


void VisualTessellatedPlanetComponent::DrawNoShadows(D3D& d3d)
{
    Shader* noShadowShader = G_ShaderManager().GetShader("Mesh_2L_1T");
    //----------------------------------------------------------------------------------------------
    // Get matrices and put in buffer format.
    ConstantBuffers::MVPBuffer matBuffer;
    matBuffer.modelMatrix       = glm::transpose(
                                    GetParent().GetTransform().GetMatrix());
    matBuffer.viewMatrix        = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
    matBuffer.projectionMatrix  = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetProjMatrix());
    // Set the buffer data using above matrices.
    noShadowShader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get light from the scene.
    const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
    // Get first light.
    if(lights.size() > 0)
    {
        LightComponent* light1 = static_cast<LightComponent*>(lights[0]);
        LightComponent* light2 = static_cast<LightComponent*>(lights[1]);
        ConstantBuffers::LightColorBuffer2 firstLight = 
        { 
            light1->GetAmbient(),
            light1->GetDiffuse(),
            light1->GetSpecular(),
            40.0f,
            glm::vec3(0.0f, 0.0f, 0.0f),

            light2->GetAmbient(),
            light2->GetDiffuse(),
            light2->GetSpecular(),
            40.0f,
            glm::vec3(0.0f, 0.0f, 0.0f)
        };

        noShadowShader->PSSetConstBufferData(d3d, std::string("LightColorBuffer"), 
                                             (void*)&firstLight, sizeof(firstLight), 0);

        // Get light positions and send to buffer.
        ConstantBuffers::LightPosBuffer2 posBuffer =
        {
            glm::vec4(light1->GetParent().GetPos(), 0.0f),
            glm::vec4(light2->GetParent().GetPos(), 0.0f)
        };

        noShadowShader->VSSetConstBufferData(d3d, std::string("LightPositionBuffer"), 
                                             (void*)&posBuffer, sizeof(posBuffer), 1);

    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get active camera and put data in CameraBufferFormat, then send to shader.
    const ConstantBuffers::CameraPosBuffer cam = 
    { 
        GetParent().GetParent().GetActiveCamera()->GetParent().GetPos(), 
        0.0f 
    };

    noShadowShader->VSSetConstBufferData(d3d, std::string("CameraBuffer"), (void*)&cam, sizeof(cam), 
                                         2);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get texture for this model and set for shader.
	ID3D11ShaderResourceView* tex = m_texture.GetTexture();
	d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    // Render sahder.
    noShadowShader->RenderShader(d3d, m_mesh.GetIndexCount());
}


void VisualTessellatedPlanetComponent::DrawWithShadows(D3D& d3d)
{
    if(m_mesh.DoesContainTanBin())
    {
        SetShader(G_ShaderManager().GetShader("PlanetTerrain"));
    }
    else
    {
        SetShader(G_ShaderManager().GetShader("PlanetTerrain"));
	}

	ConstantBuffers::TessellationBuffer tessBuffer
	{
		2.0f,
		glm::vec3(0.0f)
	};

	GetShader().HSSetConstBufferData(d3d, std::string("TessellationBuffer"), (void*)&tessBuffer, 
		sizeof(tessBuffer), 0);


	ConstantBuffers::MVPBuffer mvpBuffer
	{
		glm::transpose( GetParent().GetTransform().GetMatrix() ),
		glm::transpose( GetParent().GetParent().GetActiveCamera()->GetViewMatrix() ),
		glm::transpose( GetParent().GetParent().GetActiveCamera()->GetProjMatrix() )
	};

	GetShader().DSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&mvpBuffer, 
									 sizeof(mvpBuffer), 0);

    // Render shader.
    GetShader().RenderShader(d3d, m_mesh.GetIndexCount());
}