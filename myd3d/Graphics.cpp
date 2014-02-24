#include "Graphics.h"

#include "Scenes\Scene.h"
#include "Scenes\PlaneScene.h"
#include "Scenes\ExampleScene.h"
#include "Scenes\Example2DScene.h"

#include <AntTweakBar.h>

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen) 
    :   m_d3d(screenWidth, screenHeight, true, hwnd, fullscreen),
        m_sceneMgr(m_d3d),
        m_timer(),
        m_statsTweakBar(0),
        m_fps(0.0f)
{
    m_sceneMgr.AddScene(new Example2DScene("PlaneName", &m_sceneMgr));
    m_timer.Start();

    m_statsTweakBar = TwNewBar("Performance Stats");
    TwAddVarRO(m_statsTweakBar, "FPS", TW_TYPE_FLOAT, &m_fps, " precision=2 ");   
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

	m_sceneMgr.Draw(m_d3d);

    TwDraw();

    m_d3d.EndScene();

    return true;
}
