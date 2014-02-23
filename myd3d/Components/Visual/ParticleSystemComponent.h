#pragma once
#include "VisualComponent.h"
#include "../../Assets/Textures/Texture.h"

class ParticleSystemComponent : public VisualComponent
{
private:
    struct Particle
    {
        bool active;
        glm::vec3 position;
        glm::vec4 color;
        glm::vec3 velocity;
        float size;
    };
    struct VertexType
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec4 color;
    };

public:
    ParticleSystemComponent(D3D& d3d, const std::string& effectFile);
    explicit ParticleSystemComponent(D3D& d3d);
    ~ParticleSystemComponent(void);
private:
    ParticleSystemComponent& operator=(const ParticleSystemComponent& other) 
    {
        this->m_engagedParticles        = other.m_engagedParticles;
        this->m_maxParticleCount        = other.m_maxParticleCount;
        this->m_currentParticleCount    = 0;
        this->m_emissionFreq            = other.m_emissionFreq;
        this->m_timeBetweenEmissions    = other.m_timeBetweenEmissions;
        this->m_timeSinceLastEmission   = 0.0f;
        this->m_systemLifetime          = other.m_systemLifetime;
        this->m_currentLifetime         = other.m_currentLifetime;
        this->m_startPosition           = other.m_startPosition;
        this->m_startPositionDeviation  = other.m_startPositionDeviation;
        this->m_startVelocity           = other.m_startVelocity;
        this->m_startVelocityDeviation  = other.m_startVelocityDeviation;
        this->m_startColor              = other.m_startColor;
        this->m_startColorDeviation     = other.m_startColorDeviation;
        this->m_colorChangePerSec       = other.m_colorChangePerSec;
        this->m_startSize               = other.m_startSize;
        this->m_startSizeDeviation      = other.m_startSizeDeviation;
        this->m_sizeChangePerSec        = other.m_sizeChangePerSec;
        this->m_texture                 = other.m_texture;
        this->m_effectName              = other.m_effectName;
        this->m_vertexCount             = other.m_vertexCount;
        this->m_indexCount              = other.m_indexCount;
        this->m_vertices                = 0;
        this->m_vertexBuffer            = other.m_vertexBuffer;
        this->m_indexBuffer             = other.m_indexBuffer;
        this->m_tweakBarSetup           = this->m_tweakBarSetup;
        this->m_emitting                = false;

        return *this;
    }

    ParticleSystemComponent(const ParticleSystemComponent& other) 
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
        this->m_engagedParticles        = other.m_engagedParticles;
        this->m_maxParticleCount        = other.m_maxParticleCount;
        this->m_currentParticleCount    = 0;
        this->m_emissionFreq            = other.m_emissionFreq;
        this->m_timeBetweenEmissions    = other.m_timeBetweenEmissions;
        this->m_timeSinceLastEmission   = 0.0f;
        this->m_systemLifetime          = other.m_systemLifetime;
        this->m_currentLifetime         = other.m_currentLifetime;
        this->m_startPosition           = other.m_startPosition;
        this->m_startPositionDeviation  = other.m_startPositionDeviation;
        this->m_startVelocity           = other.m_startVelocity;
        this->m_startVelocityDeviation  = other.m_startVelocityDeviation;
        this->m_startColor              = other.m_startColor;
        this->m_startColorDeviation     = other.m_startColorDeviation;
        this->m_colorChangePerSec       = other.m_colorChangePerSec;
        this->m_startSize               = other.m_startSize;
        this->m_startSizeDeviation      = other.m_startSizeDeviation;
        this->m_sizeChangePerSec        = other.m_sizeChangePerSec;
        this->m_texture                 = other.m_texture;
        this->m_effectName              = other.m_effectName;
        this->m_vertexCount             = other.m_vertexCount;
        this->m_indexCount              = other.m_indexCount;
        this->m_vertices                = 0;
        this->m_vertexBuffer            = other.m_vertexBuffer;
        this->m_indexBuffer             = other.m_indexBuffer;
        this->m_tweakBarSetup           = this->m_tweakBarSetup;
        this->m_emitting                = false;
    }

public:
    virtual void ComponentID(componentId_t& out) const;

    virtual void Update(float time);

    virtual void Draw(D3D& d3d);

    /**
     * Prompts the system to begin. If already going, all particles removed and starts from scratch.
     */
    void Start();

    /**
     * Stops the system from emitting longer. Leaves currently emitted particles to die out.
     */
    void Stop();

    /**
     * Similar to Stop(), except this function immediately kills all particles.
     */
    void HardStop();

    const std::string& GetEffectName() const { return m_effectName; }

private:
    bool LoadFromFile(const std::string& filename, D3D& d3d);

    bool InitBuffers(D3D& d3d);

    /**
     * Called in the Update method, removes any particles that need to be removed.
     */
    void RemoveParticles(float time);
    /**
     * Immediately removes all particles from the system.
     */
    void KillAllParticles();
    /**
     * Called in the Update method. Emits particles if enough time has passed.
     */
    void EmitParticles(float time);
    /**
     * Called within EmitParticles. Initializes "newParticle" data.
     */
    void InitNewEmission(Particle& newParticle) const;

    bool UpdateBuffers(D3D& d3d) const;

    bool TweakBarSetup();

private:
    std::vector<Particle>  m_engagedParticles;
    unsigned int           m_maxParticleCount;
    unsigned int           m_currentParticleCount;
    float                  m_emissionFreq;
    float                  m_timeBetweenEmissions;
    float                  m_timeSinceLastEmission;
    float                  m_systemLifetime;
    float                  m_currentLifetime;
    glm::vec3              m_startPosition;
    glm::vec3              m_startPositionDeviation;
    glm::vec3              m_startVelocity;
    glm::vec3              m_startVelocityDeviation;
    glm::vec4              m_startColor;
    glm::vec4              m_startColorDeviation;
    glm::vec4              m_colorChangePerSec;
    float                  m_startSize;
    float                  m_startSizeDeviation;
    float                  m_sizeChangePerSec;
    Texture*               m_texture;
    std::string            m_effectName;

    int                    m_vertexCount;
    int                    m_indexCount;
    VertexType*            m_vertices;
    ID3D11Buffer*          m_vertexBuffer;
    ID3D11Buffer*          m_indexBuffer;

    bool                   m_tweakBarSetup;
    bool                   m_emitting;
};

std::istream& operator>> (std::istream& in, glm::vec3& vec);
std::istream& operator>> (std::istream& in, glm::vec4& vec);
