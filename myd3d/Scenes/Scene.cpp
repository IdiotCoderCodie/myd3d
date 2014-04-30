#include "Scene.h"
#include "../Components/Camera/CameraComponent.h"
#include "../Components/Light/LightComponent.h"
#include "SceneManager.h"
#include <algorithm>

Scene::Scene(const std::string& name, SceneManager* sceneMgr)
    :   m_Name(name),
        m_Parent(sceneMgr),
        m_Entities(),
        m_ActiveCamera(0)
{
}


Scene::~Scene()
{
    try
    {
        for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
        {
            delete *it;
            *it = 0;
        }
    }
    catch(int& e)
    {
    }

    m_Parent = 0;
}


void Scene::AddEntity(Entity* ent)
{
    m_Entities.push_back(ent);

    // Check if the entity has a light component attached.
    Component* light = ent->GetComponent("LightComponent");
	if(light)
	{
        // Cast the component to a LightComponent and push it into the lights list.
		m_Lights.push_back(light);

        // New light, create a shadow map for it.
        int screenWidth = m_Parent->GetD3DInstance().GetScreenWidth();
        int screenHeight = m_Parent->GetD3DInstance().GetScreenHeight();
        RenderTarget* newShadowMap = new RenderTarget(&m_Parent->GetD3DInstance().GetDevice(), 
                                                      screenWidth, screenHeight);
        m_shadowMaps.push_back(newShadowMap);
	}

	// If there is not already an active camera component then check if the 
	// added entity has a camera component attached to it and add this as the
	// active camera.
    if(!m_ActiveCamera)
	{
		Component* camCheck = ent->GetComponent("CameraComponent");
		if(camCheck)
		{
			m_ActiveCamera = static_cast<CameraComponent*>(camCheck);
		}
	}
}

Entity* Scene::GetEntity(const std::string& entityId)
{
    auto it = std::find_if(m_Entities.begin(), m_Entities.end(),
                [&](Entity* ent){ // Find the first element with ID matching "entityId".
                    return (!ent->GetID().compare(entityId));
                });

    if(it != m_Entities.end())
    {
        // Found a matching entity.
        return &(*(*it)); // Maybe not necessary? Derefernce itr, derefence ptr, and get address.
    }

    return nullptr;
}


void Scene::Update(double time)
{
    for (int i = 0; i < m_Entities.size(); i++)
    {
        m_Entities[i]->Update(time);
    }
    /*for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
    {
        (*it)->Update(time);
    }*/
}


void Scene::Draw(D3D& d3d)
{
    for(int i = 0; i < m_Entities.size(); i++)
    {
        m_Entities[i]->Draw(d3d);
    }
    /*for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
    {
        (*it)->Draw(d3d);
    }*/
}
