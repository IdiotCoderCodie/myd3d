#include "Example2DScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"

Example2DScene::Example2DScene(const std::string& name, SceneManager* sceneMgr)
: Scene(name, sceneMgr)
{
	D3D& d3d = GetParent().GetD3DInstance();

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

	EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", 100, 100, m_screenWidth, m_screenHeight,
		"testBitmap");

	EntityFactory::CreateOrthoFpCameraEntity(*this, -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
		-m_screenHeight / 2.0f, m_screenHeight / 2.0f, "testCam");
	
}


Example2DScene::~Example2DScene()
{
}


void Example2DScene::Update(double time)
{
	Scene::Update(time);
}


void Example2DScene::Draw(D3D& d3d)
{
	Scene::Draw(d3d);
}