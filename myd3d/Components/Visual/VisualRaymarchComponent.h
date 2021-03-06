#pragma once
#include "VisualComponent.h"
#include "../../Assets/Models/StaticMesh.h"
#include "../../Assets/Textures/Texture.h"
#include "../../RenderTarget.h"
#include <DirectXMath.h>

class VisualRaymarchComponent : public VisualComponent
{

public:
    VisualRaymarchComponent(D3D& d3d, const std::string& filename, int raymarchId);

    ~VisualRaymarchComponent(void);

    VisualRaymarchComponent& operator=(const VisualRaymarchComponent& other);

    virtual void ComponentID(componentId_t& out) const;

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
	StaticMesh m_mesh;
    int        m_raymarchId;
};

