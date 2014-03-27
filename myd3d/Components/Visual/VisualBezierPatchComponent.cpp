#include "VisualBezierPatchComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../Assets/Shaders/ShaderManager.h"
#include "../../Assets/Textures/TextureManager.h"

#include "../../glm/gtc/matrix_transform.hpp"

#include <string>

static float   m_totalTime = 0.0f;

VisualBezierPatchComponent::VisualBezierPatchComponent(D3D& d3d,
	Texture& texture, std::vector<RenderTarget*>& shadowMaps)
    : VisualComponent(),
	  m_vertexBuffer(0),
	  m_indexBuffer(0),
	  m_texture(texture),
      m_shadowMaps(shadowMaps),
      m_castShadows(false),
      m_recieveShadows(false),
	  m_tessFactor(48.0f),
	  m_tweakBarInitialized(false),
	  m_tessPartitioning(1),
	  m_terrainMagnitude(0.4f),
	  m_texelSize(0.05f),
      m_distanceBased(0),
	  m_timeElapsed(0.0f)
{
    if(!G_ShaderManager().IsLoaded())
    {
        G_ShaderManager().LoadShaders(d3d, "configFile");
    }
   SetShader(G_ShaderManager().GetShader("Normal_Shadows_Test"));

   InitBuffers(d3d);
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


VisualBezierPatchComponent::~VisualBezierPatchComponent(void)
{
}


void VisualBezierPatchComponent::InitTweakBar()
{
	TwBar* bar = GetParent().GetTweakBar();
	std::string tweakId = GetParent().GetID();

	TwAddVarRW(bar, "TessFactor", TW_TYPE_FLOAT, &m_tessFactor, "step=0.01");
	TwAddVarRW(bar, "TessPartitioning", TW_TYPE_INT32, &m_tessPartitioning, "max=3 min=0");
	//TwAddVarRW(bar, "TerrainMagnitude", TW_TYPE_FLOAT, &m_terrainMagnitude, "step=0.01");
	//TwAddVarRW(bar, "TerrainTexelSize", TW_TYPE_FLOAT, &m_texelSize, "step=0.0001");
    TwAddVarRW(bar, "DistanceBased", TW_TYPE_INT32, &m_distanceBased, "min=0 max=1");

	for (int i = 0; i < 16; i++)
	{
		TwAddVarRW(bar, std::string("CtrlPoint" + std::to_string(i)).c_str(), TW_TYPE_DIR3F, &m_controlPoints[i], "group=ControlPoints");
	}
    
	m_tweakBarInitialized = true;
}

VisualBezierPatchComponent& VisualBezierPatchComponent::operator=(const VisualBezierPatchComponent& other)
{
    m_texture;
    m_shadowMaps;
    m_castShadows = other.m_castShadows;
    m_recieveShadows = other.m_recieveShadows;
    return *this;
}


void VisualBezierPatchComponent::ComponentID(componentId_t& out) const
{ 
    out = "VisualBezierPatchComponent"; 
}


bool VisualBezierPatchComponent::InitBuffers(D3D& d3d)
{
	unsigned long* indices = new unsigned long[16];
	if (!indices)
	{
		return false;
	}

	memset(m_controlPoints, 0, sizeof(glm::vec3) * 16);

	m_controlPoints[0] = glm::vec3(-1.0f, 0.0f, -1.0f);
	m_controlPoints[1] = glm::vec3(-0.33f, 0.0f, -1.0f);
	m_controlPoints[2] = glm::vec3(0.33f, 0.0f, -1.0f);
	m_controlPoints[3] = glm::vec3(1.0f, 0.0f, -1.0f);
	m_controlPoints[4] = glm::vec3(-1.0f, 0.0f, -0.33f);
	m_controlPoints[5] = glm::vec3(-0.33f, 0.0f, -0.33f);
	m_controlPoints[6] = glm::vec3(0.33f, 0.0f, -0.33f);
	m_controlPoints[7] = glm::vec3(1.0f, 0.0f, -0.33f);
	m_controlPoints[8] = glm::vec3(-1.0f, 0.0f, 0.33f);
	m_controlPoints[9] = glm::vec3(-0.33f, 0.0f, 0.33f);
	m_controlPoints[10] = glm::vec3(0.33f, 0.0f, 0.33f);
	m_controlPoints[11] = glm::vec3(1.0f, 0.0f, 0.33f);
	m_controlPoints[12] = glm::vec3(-1.0f, 0.0f, 1.0f);
	m_controlPoints[13] = glm::vec3(-0.33f, 0.0f, 1.0f);
	m_controlPoints[14] = glm::vec3(0.33f, 0.0f, 1.0f);
	m_controlPoints[15] = glm::vec3(1.0f, 0.0f, 1.0f);

	for (int i = 0; i < 16; i++)
	{
		indices[i] = i;
	}

	// Create vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc =
	{
		sizeof(glm::vec3) * 16,		// Byte Width
		D3D11_USAGE_DYNAMIC,		// Usage
		D3D11_BIND_VERTEX_BUFFER,	// Bind flags
		D3D11_CPU_ACCESS_WRITE,		// CPUAccessFlags
		0,							// MiscFlags
		0							// StructureByteStride
	};

	D3D11_SUBRESOURCE_DATA vertexData =
	{
		m_controlPoints,	// pSysMem
		0,					// SysMemPitch
		0					// SysMemSlicePitch
	};

	HRESULT hr = d3d.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(hr))
	{
		delete[] indices;
		indices = 0;
		return false;		
	}

	// Setup index buffer.
	D3D11_BUFFER_DESC indexBufferDesc =
	{
		sizeof(unsigned long)* 16,				// ByteWidth
		D3D11_USAGE_DEFAULT,                    // Usage
		D3D11_BIND_INDEX_BUFFER,                // Bind Flags
		0,                                      // CPUAccessFlags
		0,                                      // MiscFlags
		0                                       // StructureByteStride
	};
	D3D11_SUBRESOURCE_DATA indexData =
	{
		indices,    // pSysMem
		0,          // SysMemPitch
		0           // SysMemSlicePitch
	};
	hr = d3d.GetDevice().CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(hr))
	{
		delete[] indices;
		indices = 0;
		return false;		
	}

	delete[] indices;
	indices = 0;

	return true;
}


void VisualBezierPatchComponent::UpdateBuffers(D3D& d3d)
{
	// Lock vertex buffer to send new data to it.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = d3d.GetDeviceContext().Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource);

	if (FAILED(hr))
	{
		return;// false;
	}
	
	// Send updated data. (NOTE: Currently no checking to see if the data has actually been
	// updated since the last frame, so could be sending the same data. Would be best to do
	// a check.
	glm::vec3* controlPointsPtr = (glm::vec3*)mappedResource.pData;
	memcpy(controlPointsPtr, (void*)m_controlPoints, sizeof(glm::vec3) * 16);

	d3d.GetDeviceContext().Unmap(m_vertexBuffer, 0);
}

void VisualBezierPatchComponent::Update(float timeElapsed)
{
	if(!m_tweakBarInitialized)
		InitTweakBar();
    m_totalTime += timeElapsed;
	//m_controlPoints[0].y = 0.0f + glm::cos(m_totalTime);
	//m_controlPoints[15].y = 0.0f + glm::cos(m_totalTime);
	UpdateBuffers(GetParent().GetParent().GetParent().GetD3DInstance());
}


void VisualBezierPatchComponent::ShadowPass(D3D& d3d)
{
    //if(m_castShadows)
    //{
    //   // m_mesh.Render(d3d, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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


void VisualBezierPatchComponent::Draw(D3D& d3d)
{
    //m_mesh.Render(d3d, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	// TODO: Bind buffers!

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


void VisualBezierPatchComponent::DrawNoShadows(D3D& d3d)
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
    noShadowShader->RenderShader(d3d, 16);
}


void VisualBezierPatchComponent::DrawWithShadows(D3D& d3d)
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
	/*case 0 :
		SetShader(G_ShaderManager().GetShader("TorusTessellation"));
		break;
	case 1:
		SetShader(G_ShaderManager().GetShader("TorusTessellation_FractionalEven"));
		break;
	case 2:
		SetShader(G_ShaderManager().GetShader("TorusTessellation_FractionalOdd"));
		break;
	case 3:
		SetShader(G_ShaderManager().GetShader("TorusTessellation_Pow2"));
		break;*/
	default:
		SetShader(G_ShaderManager().GetShader("BezierPatch"));
		break;
	}


	/*ConstantBuffers::TessellationBuffer tessBuffer; OLD
	tessBuffer.tesselationAmount = m_tessFactor;
	tessBuffer.padding.x		 = m_terrainMagnitude;*/

   /* ConstantBuffers::TessellationBufferDist tessBuffer;
    tessBuffer.distanceBased = m_distanceBased;
    tessBuffer.tessellationAmount = m_tessFactor;
    tessBuffer.eyePos = glm::vec4(GetParent().GetParent().GetActiveCamera()->GetParent().GetPos(), 0.0f);
    tessBuffer.modelMatrix = glm::transpose( GetParent().GetTransform().GetMatrix() );   

	GetShader().HSSetConstBufferData(d3d, std::string("TessellationBuffer"), (void*)&tessBuffer, 
		sizeof(tessBuffer), 0);*/

	// Send mvp data.
	ConstantBuffers::MVPBuffer mvpBuffer;
	mvpBuffer.modelMatrix		= glm::transpose( GetParent().GetTransform().GetMatrix() );
	mvpBuffer.viewMatrix		= glm::transpose( GetParent().GetParent().GetActiveCamera()->GetViewMatrix() );
	mvpBuffer.projectionMatrix	= glm::transpose( GetParent().GetParent().GetActiveCamera()->GetProjMatrix() );

	GetShader().DSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&mvpBuffer, 
									 sizeof(mvpBuffer), 0);


	// Light positions buffer.
	auto lights						= GetParent().GetParent().GetLights();
	ConstantBuffers::LightPosBuffer lightPosBuffer;
	
	LightComponent* light1			= static_cast<LightComponent*>(lights[0]);
	lightPosBuffer.lightPosition	= glm::vec4(light1->GetParent().GetPos(), 0.0f);

	GetShader().DSSetConstBufferData(d3d, std::string("LightPositionBuffer"), (void*)&lightPosBuffer,
									sizeof(lightPosBuffer), 1);
	

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
									(void*)&cameraPosBuffer, sizeof(cameraPosBuffer), 0);

	//ID3D11ShaderResourceView* heightMap = m_heightMap.GetTexture();
	//d3d.GetDeviceContext().DSSetShaderResources(0, 1, &heightMap);

    // Render shader.
	// TODO: Replace GetIndexCount() with 16?
	unsigned int stride = sizeof(glm::vec3);
	unsigned int offset = 0;
	d3d.GetDeviceContext().IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	d3d.GetDeviceContext().IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	d3d.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST);

    GetShader().RenderShader(d3d, 16);
}
