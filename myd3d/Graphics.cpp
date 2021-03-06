#pragma once
#include "Graphics.h"

#include "Scenes\Scene.h"
#include "Scenes\PlaneScene.h"
#include "Scenes\ExampleScene.h"
#include "Scenes\Example2DScene.h"
#include "Scenes\AdvRenderingScene.h"
#include "Scenes\TerrainDestructionScene.h"
#include "Scenes\TerrainDestructionOffworldScene.h"
#include "Scenes\TerrainDestructionCannBallScene.h"

#include <AntTweakBar.h>

extern unsigned int exeCode;

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen) 
    :   m_d3d(screenWidth, screenHeight, true, hwnd, fullscreen),
        m_sceneMgr(m_d3d),
        m_timer(),
        m_statsTweakBar(0),
        m_fps(0.0f),
        m_wireframe(false)
{
    switch(exeCode)
    {
    case 1:
        m_sceneMgr.AddScene(new TerrainDestructionScene("MainScene", &m_sceneMgr));
        break;
    case 2:
        m_sceneMgr.AddScene(new TerrainDestructionOffworldScene("MainScene", &m_sceneMgr, 1));
        break;
    case 3:
        m_sceneMgr.AddScene(new TerrainDestructionOffworldScene("MainScene", &m_sceneMgr, 2));
        break;
    case 4:
        m_sceneMgr.AddScene(new TerrainDestructionCannBallScene("MainScene", &m_sceneMgr, 1));
        break;
    case 5:
        m_sceneMgr.AddScene(new TerrainDestructionCannBallScene("MainScene", &m_sceneMgr, 2));
        break;
    }
    m_timer.Start();

    /*m_statsTweakBar = TwNewBar("Performance Stats");
    TwAddVarRO(m_statsTweakBar, "FPS", TW_TYPE_FLOAT, &m_fps, " precision=2 ");   

    TwAddVarRW(m_statsTweakBar, "Wireframe", TW_TYPE_BOOLCPP, &m_wireframe, "");*/
}


Graphics::~Graphics(void)
{
    TwTerminate();
}


bool Graphics::IsInitialized() const
{
    return m_d3d.IsInitialized();
}


bool Graphics::Frame()
{
    m_d3d.BeginScene(0.8f, 0.8f, 0.8f, 0.1f);
    
    double elapsedTime = m_timer.GetTimeInSeconds();

    m_fps = 1.0f / elapsedTime;

    m_sceneMgr.Update(elapsedTime);

    m_wireframe ? m_d3d.EnableWireframe() : m_d3d.DisableWireframe();

	m_sceneMgr.Draw(m_d3d);

    TwDraw();

    m_d3d.EndScene();

    return true;
}
