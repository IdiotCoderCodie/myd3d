#include "TerrainDestructionScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/PhysicsSystem.h"

TerrainDestructionScene::TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

	Entity* testSphere = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", 100, 100, m_screenWidth, m_screenHeight,
		"testBitmap");

	m_physicsSystem.AddCircle(testSphere, 50.0f, glm::vec2(0.0f, 100.0f));


	Entity* testSphere2 = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", 100, 100, m_screenWidth, m_screenHeight,
		"testBitmap");
	testSphere2->MoveUp(200.0f);

	m_physicsSystem.AddCircle(testSphere2, 50.0f, glm::vec2(0.0f, 0.0f));

	EntityFactory::CreateOrthoFpCameraEntity(*this, -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
		-m_screenHeight / 2.0f, m_screenHeight / 2.0f, "testCam");
	
}


TerrainDestructionScene::~TerrainDestructionScene(void)
{
}


void TerrainDestructionScene::Update(double time)
{
    Scene::Update(time);

	m_physicsSystem.Update(time);
}


void TerrainDestructionScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}