#pragma once
#include <vector>
#include <string>

#include "../D3D.h"
#include "../Entities/Entity.h"
#include "../RenderTarget.h"

class CameraComponent;

class SceneManager;

class Scene
{
public:
    Scene(const std::string& name, SceneManager* sceneMgr);
    virtual ~Scene();

    const std::string& GetSceneName() const { return m_Name; }

    void AddEntity(Entity* entity);

    const CameraComponent* GetActiveCamera() const            { return m_ActiveCamera; }
    void SetActiveCamera(CameraComponent* const newActiveCam) { m_ActiveCamera = newActiveCam; }

    const std::vector<Component*>& GetLights() const { return m_Lights; }

    SceneManager& GetParent() { return *m_Parent; }

    const std::vector<Entity*>& GetEntities() const { return m_Entities; }
    std::vector<Entity*>& GetEntities() { return m_Entities; }

    const std::vector<RenderTarget*>& GetShadowMaps() const { return m_shadowMaps; }
    std::vector<RenderTarget*>& GetShadowMaps() { return m_shadowMaps; }

    virtual void Update(double time);

    virtual void Draw(D3D& d3d);

private:
    std::string m_Name;
    SceneManager* m_Parent;
    std::vector<Entity*> m_Entities;
    std::vector<Component*> m_Lights;
    CameraComponent* m_ActiveCamera;
    std::vector<RenderTarget*> m_shadowMaps;
};

