#include "TerrainDestructionOffworldScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/PhysicsSystem.h"
#include "../Entities/Entity.h"
#include "../Assets/Textures/TextureManager.h"

#include <fstream>

#ifndef _NETWORK_DEBUGOUT
#define _NETWORK_DEBUGOUT
std::ofstream debugOut("TerrainDestructionNetworkingLog.txt");
#endif 

TerrainDestructionOffworldScene::TerrainDestructionOffworldScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

    // Camera.
    EntityFactory::CreateOrthoFpCameraEntity(*this, -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
		-m_screenHeight / 2.0f, m_screenHeight / 2.0f, "testCam");

    Texture* mainCircTex = G_TextureManager().GetTexture("cement.dds");
    if(!mainCircTex)
    {
        mainCircTex = G_TextureManager().LoadTexture(d3d, L"cement.dds", "cement.dds");
    }

    Texture* circStencil =  G_TextureManager().GetTexture("circleStencil.dds");
    if(!circStencil)
    {
        circStencil = G_TextureManager().LoadTexture(d3d, L"circleStencil.dds", "circleStencil.dds");
    }
   
    m_circleBmp = new VisualBitmapComponent(d3d, mainCircTex->GetTexture(), circStencil->GetTexture(),
                                            100, 100, m_screenWidth, m_screenHeight);

    m_circleEnt = new Entity(*this, "tehCircle");

    m_circleEnt->SetComponent(m_circleBmp);


    cerr << "test" << endl;
    if(!m_wsa.isOk())
    {
        cerr << "Error initializing wsa." << endl;
    }
    else
    {
        m_networkManager.SetTerrainDestructionOffworldScene(*this);
        m_networkManager.SetPlayerNum(1);
        m_networkManager.start();
    }
}


TerrainDestructionOffworldScene::~TerrainDestructionOffworldScene(void)
{
    m_networkManager.finish();
    m_networkManager.waitForFinish();
}


void TerrainDestructionOffworldScene::Update(double time)
{
    Scene::Update(time);

	//m_physicsSystem.Update(time);
}


void TerrainDestructionOffworldScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);

    auto circleData = m_networkManager.GetCircleData();

    for(int i = 0; i < circleData.size(); i++)
    {
        // TODO: circle data size can be bigger than number of circles. Need a number storing how
        // many circles there currently are.
        // DIRTY AS FUCK.
        m_circleEnt->SetPos(glm::vec3(circleData[i].position.x, circleData[i].position.y, 0.0f));
        m_circleEnt->Draw(d3d);
    }
}
