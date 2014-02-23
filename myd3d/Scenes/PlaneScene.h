#pragma once
#include "Scene.h"
#include "../RenderTarget.h"

class PlaneScene : public Scene
{
public:
    PlaneScene(const std::string& name, SceneManager* sceneMgr);
    ~PlaneScene(void);

    void Draw(D3D& d3d);
    void Update(double time);

private:
    void LoadPlanes(D3D& d3d);

private:
    Entity* m_drawRenderTargetEntity;
    std::vector<Entity*> m_planes;
    Entity* m_camComponent;
    Entity* m_airfieldCam;
    Entity* m_planeFollowCam;
    int m_planeToFollow;
    bool m_prevF4Pressed;
    bool m_prevPPressed;
    bool m_paused;
};
