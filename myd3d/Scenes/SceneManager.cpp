#include <cassert>

#include "SceneManager.h"
#include "../Assets/Shaders/ShaderManager.h"


SceneManager::SceneManager(D3D& d3d)
    :   m_Scenes(), 
        m_ActiveScene(0),
        m_d3d(d3d)
{
    G_ShaderManager().LoadShaders(d3d, "");
}


SceneManager::~SceneManager()
{
    try
    {
        ClearScenes();
    }
    catch (int& e)
    {
        assert(false);
    }
}


SceneManager& SceneManager::operator=(const SceneManager& other)
{
    m_Scenes = other.m_Scenes;
    m_ActiveScene = other.m_ActiveScene;
    return *this;
}


bool SceneManager::SetActiveScene(const std::string& sceneName)
{
    if(DoesSceneExist(sceneName))
    {
        m_ActiveScene = m_Scenes[sceneName];
        return true;
    }
    return false;
}


bool SceneManager::SetActiveScene(Scene* const scene)
{
    // If AddScene works set m_ActiveScene to point to this scene.
    if(AddScene(scene))
    {
        m_ActiveScene = scene;
        return true;
    }
    
    // If AddScene fails, there is already a Scene with the same name. (Possibly the same scene).
    return false;
}


bool SceneManager::DoesSceneExist(const std::string& sceneName) const
{
    if(m_Scenes.find(sceneName) != m_Scenes.end())
        return true;
    
    return false;
}


bool SceneManager::AddScene(Scene* const sceneToAdd)
{
    if(!DoesSceneExist(sceneToAdd->GetSceneName()))
    { // No scene exists in the current context with that name.
        m_Scenes[sceneToAdd->GetSceneName()] = sceneToAdd;
        if(!m_ActiveScene)
        {
            m_ActiveScene = sceneToAdd;
        }
        return true;
    }
    else
    { // A scene exists with that name.
        return false;
    }
}


void SceneManager::ClearScenes()
{
    for(auto it = m_Scenes.begin(); it != m_Scenes.end(); ++it)
    { // Delete all scenes.
        delete it->second;
        it->second = 0;
    }
    // Clear the scenes map.
    m_Scenes.clear();
}


void SceneManager::Update(double time)
{
    m_ActiveScene->Update(time);
}


void SceneManager::Draw(D3D& d3d)
{
    m_ActiveScene->Draw(d3d);
}