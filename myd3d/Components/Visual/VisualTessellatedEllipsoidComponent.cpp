#include "VisualTessellatedEllipsoidComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../Assets/Shaders/ShaderManager.h"
#include "../../Assets/Textures/TextureManager.h"

#include "../../glm/gtc/matrix_transform.hpp"

static float   m_totalTime = 0.0f;

VisualTessellatedEllipsoidComponent::VisualTessellatedEllipsoidComponent(D3D& d3d, const std::string& filename,
	Texture& texture, Texture& heightMap, std::vector<RenderTarget*>& shadowMaps, float a, float b, float c)
    : VisualComponent(),
      m_mesh(filename, d3d, false), 
	  m_texture(texture),
	  m_heightMap(heightMap), 
      m_shadowMaps(shadowMaps),
      m_castShadows(false),
      m_recieveShadows(false),
	  m_tessFactor(48.0f),
	  m_tweakBarInitialized(false),
	  m_tessPartitioning(1),
	  m_terrainMagnitude(0.4f),
	  m_texelSize(0.05f),
      m_distanceBased(0),
      m_a(a),
      m_b(b),
      m_c(c),
	  m_color(1.0f, 1.0f, 1.0f),
	  m_shininess(128.0f)
{
    if(!G_ShaderManager().IsLoaded())
    {
        G_ShaderManager().LoadShaders(d3d, "configFile");
    }
   SetShader(G_ShaderManager().GetShader("Normal_Shadows_Test"));
}

//
//VisualTessellatedPlanetComponent::VisualTessellatedPlanetComponent(D3D& d3d, const std::string& filename, 
//	Texture& texture, Texture& heightMap, std::vector<RenderTarget*>& shadowMaps)
//    : VisualComponent(),
//      m_mesh(filename, d3d, true),
//      m_texture(texture),
//	  m_heightMap(heightMap),
//      m_shadowMaps(shadowMaps),
//      m_castShadows(false),
//      m_recieveShadows(false),
//	  m_tessFactor(1.0f),
//	  m_tweakBarInitialized(false),
//	  m_tessPartitioning(1)
//{
//    if(!G_ShaderManager().IsLoaded())
//    {
//        G_ShaderManager().LoadShaders(d3d, "configFile");
//    }
//    SetShader(G_ShaderManager().GetShader("Normal_Shadows_Test"));
//
//
//}


VisualTessellatedEllipsoidComponent::~VisualTessellatedEllipsoidComponent(void)
{
}


void VisualTessellatedEllipsoidComponent::InitTweakBar()
{
	TwBar* bar = GetParent().GetTweakBar();
	std::string tweakId = GetParent().GetID();

	TwAddVarRW(bar, "TessFactor", TW_TYPE_FLOAT, &m_tessFactor, "step=0.01");
	TwAddVarRW(bar, "TessPartitioning", TW_TYPE_INT32, &m_tessPartitioning, "max=3 min=0");
	//TwAddVarRW(bar, "TerrainMagnitude", TW_TYPE_FLOAT, &m_terrainMagnitude, "step=0.01");
	//TwAddVarRW(bar, "TerrainTexelSize", TW_TYPE_FLOAT, &m_texelSize, "step=0.0001");
    TwAddVarRW(bar, "DistanceBased", TW_TYPE_INT32, &m_distanceBased, "min=0 max=1");
    TwAddVarRW(bar, "a", TW_TYPE_FLOAT, &m_a, "step=0.01");
    TwAddVarRW(bar, "b", TW_TYPE_FLOAT, &m_b, "step=0.01");
    TwAddVarRW(bar, "c", TW_TYPE_FLOAT, &m_c, "step=0.01");
	TwAddVarRW(bar, "MatColor", TW_TYPE_COLOR3F, &m_color, "");
	TwAddVarRW(bar, "MatShininess", TW_TYPE_FLOAT, &m_shininess, "min=1.0 max=128.0 step=0.1");
	m_tweakBarInitialized = true;
}

VisualTessellatedEllipsoidComponent& VisualTessellatedEllipsoidComponent::operator=(const VisualTessellatedEllipsoidComponent& other)
{
    m_mesh = other.m_mesh;
    m_texture;
    m_heightMap;
    m_shadowMaps;
    m_castShadows = other.m_castShadows;
    m_recieveShadows = other.m_recieveShadows;
    return *this;
}


void VisualTessellatedEllipsoidComponent::ComponentID(componentId_t& out) const
{ 
    out = "VisualTessellatedEllipsoidComponent"; 
}


void VisualTessellatedEllipsoidComponent::Update(float timeElapsed)
{
	if(!m_tweakBarInitialized)
		InitTweakBar();
    m_totalTime += timeElapsed;
}


void VisualTessellatedEllipsoidComponent::ShadowPass(D3D& d3d)
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


void VisualTessellatedEllipsoidComponent::Draw(D3D& d3d)
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


void VisualTessellatedEllipsoidComponent::DrawNoShadows(D3D& d3d)
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


void VisualTessellatedEllipsoidComponent::DrawWithShadows(D3D& d3d)
{
    /*if(m_mesh.DoesContainTanBin())
    {
        SetShader(G_ShaderManager().GetShader("PlanetTerrain"));
    }
    else
    {
        SetShader(G_ShaderManager().GetShader("PlanetTerrain"));
	}*/


	switch(m_tessPartitioning)
	{
	case 0 :
		SetShader(G_ShaderManager().GetShader("EllipsoidTessellation"));
		break;
	case 1:
		SetShader(G_ShaderManager().GetShader("EllipsoidTessellation_FractionalEven"));
		break;
	case 2:
		SetShader(G_ShaderManager().GetShader("EllipsoidTessellation_FractionalOdd"));
		break;
	case 3:
		SetShader(G_ShaderManager().GetShader("EllipsoidTessellation_Pow2"));
		break;
	default:
		break;
	}


	/*ConstantBuffers::TessellationBuffer tessBuffer; OLD
	tessBuffer.tesselationAmount = m_tessFactor;
	tessBuffer.padding.x		 = m_terrainMagnitude;*/

    ConstantBuffers::TessellationBufferDist tessBuffer;
    tessBuffer.distanceBased = m_distanceBased;
    tessBuffer.tessellationAmount = m_tessFactor;
    tessBuffer.eyePos = glm::vec4(GetParent().GetParent().GetActiveCamera()->GetParent().GetPos(), 0.0f);
    tessBuffer.modelMatrix = glm::transpose( GetParent().GetTransform().GetMatrix() );   

	GetShader().HSSetConstBufferData(d3d, std::string("TessellationBuffer"), (void*)&tessBuffer, 
		sizeof(tessBuffer), 0);

	// Send mvp data.
	ConstantBuffers::MVPBuffer mvpBuffer;
	mvpBuffer.modelMatrix		= glm::transpose( GetParent().GetTransform().GetMatrix() );
	mvpBuffer.viewMatrix		= glm::transpose( GetParent().GetParent().GetActiveCamera()->GetViewMatrix() );
	mvpBuffer.projectionMatrix	= glm::transpose( GetParent().GetParent().GetActiveCamera()->GetProjMatrix() );

	GetShader().DSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&mvpBuffer, 
									 sizeof(mvpBuffer), 0);


	/*ConstantBuffers::TerrainBuffer terrainBuffer;
	terrainBuffer.terrainHeight = m_terrainMagnitude;
	terrainBuffer.padding		= m_texelSize;

	GetShader().DSSetConstBufferData(d3d, std::string("TerrainBuffer"), (void*)&terrainBuffer,
									 sizeof(terrainBuffer), 1);*/
   /* ConstantBuffers::TorusBuffer torusBuffer;
    torusBuffer.innerRadius = m_innerRadius;
    torusBuffer.tubeRadius  = m_tubeRadius;
    GetShader().DSSetConstBufferData(d3d, std::string("EllipsoidBuffer"), (void*)&torusBuffer,
									 sizeof(torusBuffer), 1);*/
    ConstantBuffers::EllipsoidBuffer ellipsoidBuffer;
    ellipsoidBuffer.a = m_a;
    ellipsoidBuffer.b = m_b;
    ellipsoidBuffer.c = m_c;
    GetShader().DSSetConstBufferData(d3d, std::string("EllipsoidBuffer"), (void*)&ellipsoidBuffer,
                                    sizeof(ellipsoidBuffer), 1);

	// Light positions buffer.
	auto lights						= GetParent().GetParent().GetLights();
	ConstantBuffers::LightPosBuffer lightPosBuffer;
	
	LightComponent* light1			= static_cast<LightComponent*>(lights[0]);
	lightPosBuffer.lightPosition	= glm::vec4(light1->GetParent().GetPos(), 0.0f);

	GetShader().DSSetConstBufferData(d3d, std::string("LightPositionBuffer"), (void*)&lightPosBuffer,
									sizeof(lightPosBuffer), 2);
	

	// Light buffer.
	ConstantBuffers::Light lightsBuffer[1];
	for (size_t i = 0; i < 1; i++)
	{
		LightComponent* light = static_cast<LightComponent*>(lights[i]);
		lightsBuffer[i].enabled = 1;
		lightsBuffer[i].shadows = 0;
		lightsBuffer[i].position = glm::vec4(light->GetParent().GetPos(), 1.0f);
		lightsBuffer[i].ambient = light->GetAmbient();
		lightsBuffer[i].diffuse = light->GetDiffuse();
		lightsBuffer[i].specular = light->GetSpecular();
		lightsBuffer[i].spotCutoff = glm::radians(light->GetSpotCutoff());
		lightsBuffer[i].spotDirection = light->GetParent().GetTransform().GetForward();
		lightsBuffer[i].spotExponent = light->GetSpotExponent();
		lightsBuffer[i].attenuation = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	GetShader().SetStructuredBufferData(d3d, std::string("LightBuffer"), (void*)&lightsBuffer,
		sizeof(ConstantBuffers::Light) * 1);

	ID3D11ShaderResourceView* lightBufferSRV = GetShader().GetBufferSRV(std::string("LightBuffer"));
	d3d.GetDeviceContext().PSSetShaderResources(2, 1, &lightBufferSRV);

	// Camera Buffer
	ConstantBuffers::CameraPosBuffer cameraPosBuffer;
	cameraPosBuffer.cameraPos = GetParent().GetParent().GetActiveCamera()->GetParent().GetPos();
	GetShader().PSSetConstBufferData(d3d, std::string("CameraPosBuffer"),
									(void*)&cameraPosBuffer, sizeof(cameraPosBuffer), 1);

	ConstantBuffers::MaterialBuffer matBuffer;
	matBuffer.color = m_color;
	matBuffer.shininess = m_shininess;
	GetShader().PSSetConstBufferData(d3d, std::string("MaterialBuffer"),
		(void*)&matBuffer, sizeof(matBuffer), 0);

	ID3D11ShaderResourceView* heightMap = m_heightMap.GetTexture();
	d3d.GetDeviceContext().DSSetShaderResources(0, 1, &heightMap);

    // Render shader.
    GetShader().RenderShader(d3d, m_mesh.GetIndexCount());
}
