#include "ParticleSystemComponent.h"
#include "../Component.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../../Assets/Textures/TextureManager.h"
#include "../../Components/Camera/CameraComponent.h"
#include "../../d3d_safe_release.h"
#include "../../glm/gtc/constants.hpp"
#include "../../Math/Frame.h"

#include <algorithm>
#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

//extern TextureManager G_TextureManager; 

ParticleSystemComponent::ParticleSystemComponent(D3D& d3d, const std::string& effectFile)
    : m_engagedParticles(),
      m_maxParticleCount(),
      m_currentParticleCount(),
      m_emissionFreq(),
      m_timeBetweenEmissions(),
      m_timeSinceLastEmission(),
      m_systemLifetime(),
      m_currentLifetime(0.0f),
      m_startPosition(),
      m_startPositionDeviation(),
      m_startVelocity(),
      m_startVelocityDeviation(),
      m_startColor(),
      m_startColorDeviation(),
      m_colorChangePerSec(),
      m_startSize(),
      m_startSizeDeviation(),
      m_sizeChangePerSec(),
      m_texture(0),
      m_effectName("NoEffectLoaded"),
      m_vertexCount(0),
      m_indexCount(0),
      m_vertices(0),
      m_vertexBuffer(0),
      m_indexBuffer(0),
      m_tweakBarSetup(false),  
      m_emitting(false)
{
    LoadFromFile("Assets\\ParticleEffects\\" + effectFile, d3d);
    InitBuffers(d3d);
    SetShader(G_ShaderManager().GetShader("Particle"));
    //Start();
}


ParticleSystemComponent::ParticleSystemComponent(D3D& d3d)
    : m_engagedParticles(),
      m_maxParticleCount(1000),
      m_currentParticleCount(0),
      m_emissionFreq(40.0f),
      m_timeBetweenEmissions(1.0f / m_emissionFreq),
      m_timeSinceLastEmission(0.0f),
      m_systemLifetime(10.0f),
      m_currentLifetime(0.0f),
      m_startPosition(0.0f, 0.0f, 0.0f),
      m_startPositionDeviation(0.2f, 0.0f, 0.2f),
      m_startVelocity(0.0f, 2.0f, 0.0f),
      m_startVelocityDeviation(0.3f, 0.1f, 0.3f),
      m_startColor(1.0f, 1.0f, 0.5f, 1.0f),
      m_startColorDeviation(0.1f, 0.1f, 0.0f, 1.0f),
      m_colorChangePerSec(-1.20f, -1.6f, -1.6f, -1.6f),
      m_startSize(0.30f),
      m_startSizeDeviation(0.1f),
      m_sizeChangePerSec(0.05f),
      m_texture(0),     
      m_effectName("NoEffectLoaded"),
      m_vertexCount(0),
      m_indexCount(0),
      m_vertices(0),
      m_vertexBuffer(0),
      m_indexBuffer(0),
      m_tweakBarSetup(false),
      m_emitting(false)
{
    m_texture = G_TextureManager().LoadTexture(d3d, L"flame.dds", "flame.dds");
    //m_texture = G_TextureManager.GetTexture("particle.dds");
    
    InitBuffers(d3d);

    SetShader(G_ShaderManager().GetShader("Particle"));

}


ParticleSystemComponent::~ParticleSystemComponent(void)
{
    try
    {
        d3d_safe_release(m_vertexBuffer);
        d3d_safe_release(m_indexBuffer);
    }
    catch(int& e)
    {
    }
    delete[] m_vertices;
}


void ParticleSystemComponent::ComponentID(componentId_t& out) const 
{ 
    out = "ParticleSystemComponent"; 
}


bool ParticleSystemComponent::LoadFromFile(const std::string& filename, D3D& d3d)
{
    std::ifstream input(filename);
    if(!input.is_open())
        return false;

    std::string line;
    std::istringstream value_iss;
    std::string token;
    std::string value;
    while(input.good())
    {
        std::getline(input, token, '=');
        std::getline(input, value);
        value_iss = std::istringstream(value);

        if(!token.compare("ParticleEffect"))
        {
            m_effectName = value;
        }
        else if(!token.compare("MaxParticleCount"))
        {
            value_iss >> m_maxParticleCount;
        }
        else if(!token.compare("SystemLifetime"))
        {
            value_iss >> m_systemLifetime;
        }
        else if(!token.compare("EmissionFreq"))
        {
            value_iss >> m_emissionFreq;
            m_timeBetweenEmissions = 1.0f / m_emissionFreq;
        }
        else if(!token.compare("StartPosition"))
        {
            value_iss >> m_startPosition;
        }
        else if(!token.compare("StartPositionDeviation"))
        {
            value_iss >> m_startPositionDeviation;
        }
        else if(!token.compare("StartVelocity"))
        {
            value_iss >> m_startVelocity;
        }
        else if(!token.compare("StartVelocityDeviation"))
        {
            value_iss >> m_startVelocityDeviation;
        }
        else if(!token.compare("StartColor"))
        {
            value_iss >> m_startColor;
        }
        else if(!token.compare("ColorChangePerSec"))
        {
            value_iss >> m_colorChangePerSec;
        }
        else if(!token.compare("StartSize"))
        {
            value_iss >> m_startSize;
        }
        else if(!token.compare("StartSizeDeviation"))
        {
            value_iss >> m_startSizeDeviation;
        }
        else if(!token.compare("SizeChangePerSec"))
        {
            value_iss >> m_sizeChangePerSec;
        }
        else if(!token.compare("Texture"))
        {

            std::string texStr;
            value_iss >> texStr;
            std::wstring wideStr = std::wstring(texStr.begin(), texStr.end());          
            
            m_texture = G_TextureManager().LoadTexture(d3d, &wideStr[0], texStr);
        }
    }
}


bool ParticleSystemComponent::InitBuffers(D3D& d3d)
{
    m_vertexCount = m_maxParticleCount * 6;
    m_indexCount = m_vertexCount;

    // Create vertices array.
    m_vertices = new VertexType[m_vertexCount];
    if(!m_vertices)
    {
        return false;
    }

    // Create indices array.
    unsigned long* indices = new unsigned long[m_indexCount];
    if(!indices)
    {
        return false;
    }

    // Init vertex data to 0's.
    memset(m_vertices, 0, sizeof(VertexType) * m_vertexCount);

    // Init index array.
    for (int i = 0; i < m_indexCount; i++)
    {
        indices[i] = i;
    }

    //----------------------------------------------------------------------------------------------
    // Setup vertex buffer and then create.
    D3D11_BUFFER_DESC vertexBufferDesc = 
    {
        sizeof(VertexType) * m_vertexCount,     // ByteWidth
        D3D11_USAGE_DYNAMIC,                    // Usage
        D3D11_BIND_VERTEX_BUFFER,               // Bind Flags
        D3D11_CPU_ACCESS_WRITE,                 // CPUAccessFlags
        0,                                      // MiscFlags
        0                                       // StructureByteStride
    };
    D3D11_SUBRESOURCE_DATA vertexData = 
    {
        m_vertices,     // pSysMem
        0,              // SysMemPitch
        0               // SysMemSlicePitch
    };
    HRESULT hr = d3d.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if(FAILED(hr))
    {
        return false;
        delete[] indices;
        indices = 0;
    }
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Setup index buffer and then create.
    D3D11_BUFFER_DESC indexBufferDesc = 
    {
        sizeof(unsigned long) * m_indexCount,   // ByteWidth
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
    hr = d3d.GetDevice().CreateBuffer(&indexBufferDesc, & indexData, &m_indexBuffer);
    if(FAILED(hr))
    {
        return false;
        delete[] indices;
        indices = 0;
    }
    //----------------------------------------------------------------------------------------------

    delete[] indices;
    indices = 0;

    return true;
}


void ParticleSystemComponent::Update(float time)
{
    if(!m_tweakBarSetup)
    {
        m_tweakBarSetup = TweakBarSetup();
    }

    RemoveParticles(time);

    /* The following is for the Stop() method to work as required, we want to still remove and 
       update current particles but not emit anymore. */
    if(m_emitting)
    {
        EmitParticles(time);
    }

    UpdateBuffers(GetParent().GetParent().GetParent().GetD3DInstance());

    for(Particle& p : m_engagedParticles)
    {
        p.position = p.position +  (p.velocity * time);
        p.color += m_colorChangePerSec * time;
        p.size += m_sizeChangePerSec * time;
    }

    // Update current lifetime of the system, then check if it's been going long enough.
    if(m_systemLifetime > 0.01f && m_emitting)
    {
        m_currentLifetime += time;
        if(m_currentLifetime > m_systemLifetime)
        {
            Stop();
        }
    }
}


void ParticleSystemComponent::Draw(D3D& d3d)
{
    //----------------------------------------------------------------------------------------------
    // Get matrices and put in buffer format.
    
    // Billboarding.
    glm::vec3 camPos = GetParent().GetParent().GetActiveCamera()->GetParent().GetPos();
    float angle = atan2(this->GetParent().GetPos().x - camPos.x, 
                            this->GetParent().GetPos().z - camPos.z) * (180.0f / glm::pi<float>());
   
    // Only need the position. Scrap any rotation and crap.
    Frame transform = Frame();
    transform.SetPosition(GetParent().GetTransform().GetPosition() - m_startPosition);
    transform.RotateGlobalY(180.0f+angle);

    ConstantBuffers::MVPBuffer matBuffer =
    {
        glm::transpose(transform.GetMatrix()),
        glm::transpose(GetParent().GetParent().GetActiveCamera()->GetViewMatrix()),
        glm::transpose(GetParent().GetParent().GetActiveCamera()->GetProjMatrix())
    };

    GetShader().VSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&matBuffer, 
                                   sizeof(matBuffer), 0);

    // Set texture.
    ID3D11ShaderResourceView* tex = m_texture->GetTexture();
    d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);

    // Set vertex buffer and index buffer to active.
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;
    d3d.GetDeviceContext().IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    d3d.GetDeviceContext().IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    d3d.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    d3d.EnableAlphaBlending();
    //d3d.TurnZBufferOff();
    GetShader().RenderShader(d3d, m_indexCount);
    d3d.DisableAlphaBlending();
    //d3d.TurnZBufferOn();
}


void ParticleSystemComponent::RemoveParticles(float time)
{
    // Need time in the future to check life times of particles. Do this to stop parasoft
    // whining.
    time;
    // Currently, for testing, remove all particles which go so far away from the start. (3.0).
    for(auto it = m_engagedParticles.begin(); it != m_engagedParticles.end(); )
    {
        if(it->color.r < 0.01f && it->color.g < 0.01f && it->color.b < 0.01f)
        {
            it = m_engagedParticles.erase(it);
            m_currentParticleCount--;
        }
        else
        {
            ++it;
        }
    }
}


void ParticleSystemComponent::KillAllParticles()
{
    m_engagedParticles.clear();
    m_currentParticleCount = 0;
}


void ParticleSystemComponent::EmitParticles(float time)
{
    m_timeSinceLastEmission += time;

    unsigned int particlesToEmit = (unsigned int)(m_timeSinceLastEmission / m_timeBetweenEmissions);
    if(!particlesToEmit)
    {
        // Not ready to emit any particles yet.
        return;
    }

    m_timeSinceLastEmission -= m_timeBetweenEmissions;

    for (int i = 0; (i < particlesToEmit) && (m_currentParticleCount < m_maxParticleCount); i++)
    {
        Particle newParticle;
        InitNewEmission(newParticle);
            
        // Find where the particle should go (ordered back->front).
        auto it = std::find_if(m_engagedParticles.begin(), m_engagedParticles.end(), 
                                [&](Particle const& particle) {
                                    return newParticle.position.z < particle.position.z;
                                });
          
        // Insert the new particle in designated position.
        m_engagedParticles.insert(it, newParticle);
        m_currentParticleCount++;
    }
}


void ParticleSystemComponent::InitNewEmission(Particle& newEmission) const
{
    newEmission.active = true;

    newEmission.position = m_startPosition;
    newEmission.position.x += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startPositionDeviation.x;
    newEmission.position.y += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startPositionDeviation.y;
    newEmission.position.z += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startPositionDeviation.z;

    newEmission.velocity = m_startVelocity;
    newEmission.velocity.x += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startVelocityDeviation.x;
    newEmission.velocity.y += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startVelocityDeviation.y;
    newEmission.velocity.z += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startVelocityDeviation.z;

    newEmission.color = m_startColor;
    newEmission.color.x += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startColorDeviation.x;
    newEmission.color.y += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startColorDeviation.y;
    newEmission.color.z += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startColorDeviation.z;

    newEmission.size = m_startSize;
    newEmission.size += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startSizeDeviation;
   /* newEmission.color.x += (((float)rand()-(float)rand()) / RAND_MAX) * m_startColorDeviation.x;
    newEmission.color.y += (((float)rand()-(float)rand()) / RAND_MAX) * m_startColorDeviation.y;
    newEmission.color.z += (((float)rand()-(float)rand()) / RAND_MAX) * m_startColorDeviation.z;*/
}


bool ParticleSystemComponent::UpdateBuffers(D3D& d3d) const
{
    memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

    int index = 0;

    // Re-fill vertex buffer with updated info.
    for(Particle p : m_engagedParticles)
    {
        // Bottom left.
        m_vertices[index].position = p.position + glm::vec3(-p.size, -p.size, 0.0f);
        m_vertices[index].uv       = glm::vec2(0.0f, 1.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Top left.
        m_vertices[index].position = p.position + glm::vec3(-p.size, +p.size, 0.0f);
        m_vertices[index].uv       = glm::vec2(0.0f, 0.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Bottom right.
        m_vertices[index].position = p.position + glm::vec3(+p.size, -p.size, 0.0f);
        m_vertices[index].uv       = glm::vec2(1.0f, 1.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Bottom right.
        m_vertices[index].position = p.position + glm::vec3(+p.size, -p.size, 0.0f);
        m_vertices[index].uv       = glm::vec2(1.0f, 1.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Top left.
        m_vertices[index].position = p.position + glm::vec3(-p.size, +p.size, 0.0f);
        m_vertices[index].uv       = glm::vec2(0.0f, 0.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Top right.
        m_vertices[index].position = p.position + glm::vec3(+p.size, +p.size, 0.0f);
        m_vertices[index].uv       = glm::vec2(1.0f, 0.0f);
        m_vertices[index].color    = p.color;
        index++;
    }

    // Lock the vertex buffer to send data to it.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = d3d.GetDeviceContext().Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, 
                                            &mappedResource);
    if(FAILED(hr))
    {
        return false;
    }

    VertexType* verticesPtr = (VertexType*)mappedResource.pData;

    memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType) * m_vertexCount));

    // Unlock the vertex buffer.
    d3d.GetDeviceContext().Unmap(m_vertexBuffer, 0);

    return true;
}


bool ParticleSystemComponent::TweakBarSetup()
{
    TwBar* tweakBar = GetParent().GetTweakBar();
    if(!tweakBar)
    {
        return false;
    }

    std::string tweakId = GetParent().GetID();

    TwAddVarRO(tweakBar, "Particle Count:", TW_TYPE_INT32, &m_currentParticleCount, 
               "group='ParticleSystem'");
    TwAddVarRW(tweakBar, "Emit Every (s):", TW_TYPE_FLOAT, &m_timeBetweenEmissions, 
               "group='ParticleSystem' step=0.01");
    
    // Start Position
    TwAddVarRW(tweakBar, "StartPos X", TW_TYPE_FLOAT, &m_startPosition.x, 
               "group='StartPosition' step=0.1");
    TwAddVarRW(tweakBar, "StartPos Y", TW_TYPE_FLOAT, &m_startPosition.y, 
               "group='StartPosition' step=0.1");
    TwAddVarRW(tweakBar, "StartPos Z", TW_TYPE_FLOAT, &m_startPosition.z, 
               "group='StartPosition' step=0.1");
    TwDefine((tweakId + "/StartPosition group='ParticleSystem'").c_str());

    // Start PositionDeviation
    TwAddVarRW(tweakBar, "StartPosDev X", TW_TYPE_FLOAT, &m_startPositionDeviation.x, 
               "group='StartPositionDeviation' step=0.1");
    TwAddVarRW(tweakBar, "StartPosDev Y", TW_TYPE_FLOAT, &m_startPositionDeviation.y, 
               "group='StartPositionDeviation' step=0.1");
    TwAddVarRW(tweakBar, "StartPosDev Z", TW_TYPE_FLOAT, &m_startPositionDeviation.z, 
               "group='StartPositionDeviation' step=0.1");
    TwDefine((tweakId + "/StartPositionDeviation group='ParticleSystem'").c_str());

    // Start Velocity.
    TwAddVarRW(tweakBar, "StartVelocity", TW_TYPE_DIR3F, &m_startVelocity, 
               "group='ParticleSystem'");

    // Start Velocity Deviation
    TwAddVarRW(tweakBar, "X", TW_TYPE_FLOAT, &m_startVelocityDeviation.x, 
               "group='StartVelocityDeviation' step=0.1");
    TwAddVarRW(tweakBar, "Y", TW_TYPE_FLOAT, &m_startVelocityDeviation.y, 
               "group='StartVelocityDeviation' step=0.1");
    TwAddVarRW(tweakBar, "Z", TW_TYPE_FLOAT, &m_startVelocityDeviation.z, 
               "group='StartVelocityDeviation' step=0.1");
    TwDefine((tweakId + "/StartVelocityDeviation group='ParticleSystem'").c_str());


    // Start Color
    TwAddVarRW(tweakBar, "StartColor", TW_TYPE_COLOR4F, &m_startColor, 
               "group='Color'");
    TwAddVarRW(tweakBar, "StartColorDev", TW_TYPE_COLOR4F, &m_startColorDeviation, 
               "group='Color'");
    TwAddVarRW(tweakBar, "Red", TW_TYPE_FLOAT, &m_colorChangePerSec.x, 
               "group='ColorChangePerSec' step=0.01");
    TwAddVarRW(tweakBar, "Green", TW_TYPE_FLOAT, &m_colorChangePerSec.y, 
               "group='ColorChangePerSec' step=0.01");
    TwAddVarRW(tweakBar, "Blue", TW_TYPE_FLOAT, &m_colorChangePerSec.z, 
               "group='ColorChangePerSec' step=0.01");
    TwDefine((tweakId + "/ColorChangePerSec group='Color'").c_str());
    TwDefine((tweakId + "/Color group='ParticleSystem'").c_str());

    TwAddVarRW(tweakBar, "StartSize", TW_TYPE_FLOAT, &m_startSize, 
               "group='Size' step=0.01 ");
    TwAddVarRW(tweakBar, "StartSizeDev", TW_TYPE_FLOAT, &m_startSizeDeviation, 
               "group='Size' step=0.01 ");
    TwAddVarRW(tweakBar, "sizeChangePerSec", TW_TYPE_FLOAT, &m_sizeChangePerSec, 
               "group='Size' step=0.01 ");
    TwDefine((tweakId + "/Size group='ParticleSystem'").c_str());

    TwDefine((tweakId + "/ParticleSystem opened=false ").c_str());
    return true;
}


void ParticleSystemComponent::Start()
{
    m_currentLifetime = 0.0f;
    KillAllParticles();
    m_emitting = true;
}


void ParticleSystemComponent::Stop()
{
    m_currentLifetime = 0.0f;
    m_emitting = false;
}

void ParticleSystemComponent::HardStop()
{
    m_currentLifetime = 0.0f;
    m_emitting = false;
    KillAllParticles();
}

std::istream& operator>> (std::istream& in, glm::vec3& vec)
{
    // {x, y, z}
    char ch;
    in >> ch >> vec.x >> ch >> vec.y >> ch >> vec.z >> ch;
    return in;
}

std::istream& operator>> (std::istream& in, glm::vec4& vec)
{
    // {x, y, z, w}
    char ch;
    in >> ch >> vec.x >> ch >> vec.y >> ch >> vec.z >> ch >> vec.w >> ch;
    return in;
}
