#pragma once
#include "VisualComponent.h"
#include "../../Assets/Models/StaticMesh.h"
#include "../../Assets/Textures/Texture.h"
#include "../../RenderTarget.h"
#include <DirectXMath.h>

class VisualMeshComponent : public VisualComponent
{

public:
    VisualMeshComponent(D3D& d3d, const std::string& filename, Texture& texture, 
                        std::vector<RenderTarget*>& shadowMaps);
    VisualMeshComponent(D3D& d3d, const std::string& meshFilename, Texture& texture,
                        Texture& bmpMap, std::vector<RenderTarget*>& shadowMaps);
    ~VisualMeshComponent(void);

    VisualMeshComponent& operator=(const VisualMeshComponent& other);

    virtual void ComponentID(componentId_t& out) const;

    bool DoesCastShadows() const    { return m_castShadows; }
    void EnableCastShadows()        { m_castShadows = true; }
    void DisableCastShadows()       { m_castShadows = false; }

    bool DoesRecieveShadows() const { return m_recieveShadows; }
    void EnableRecieveShadows()     { m_recieveShadows = true; }
    void DisableRecieveShadows()    { m_recieveShadows = false; }

    virtual void Update(float timeElapsed);

    /**
     * Shadow pass is done before the actual rendering. This is where the object is rendered to the 
     * provided shadow map (depth map). Only drawn into the shadow map if casting of shadows by this
     * mesh has been enabled.
     */
    void ShadowPass(D3D& d3d);

    virtual void Draw(D3D& d3d);

private:
    /**
     * DrawNoShadows is called from the main Draw function if recieving shadows is disabled, which
     * it is by default.
     */
    void DrawNoShadows(D3D& d3d);
    /**
     * DrawWithShadows is called from the main Draw function if recieving shadows have been enabled 
     * for this mesh, call EnableRecieveShadows() to do this.
     */
    void DrawWithShadows(D3D& d3d);

private:
    StaticMesh                  m_mesh;
	Texture&                    m_texture;
    Texture&                    m_bumpTexture;
    std::vector<RenderTarget*>& m_shadowMaps;
    bool                        m_castShadows;
    bool                        m_recieveShadows;
};

