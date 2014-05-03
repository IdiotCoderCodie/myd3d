#include "TerrainDestructionCannBallScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/PhysicsSystem.h"
#include "../Entities/Entity.h"
#include "../Assets/Textures/TextureManager.h"
#include "TerrainDestructionConsts.h"

#include <fstream>

//#ifndef _NETWORK_DEBUGOUT
//#define _NETWORK_DEBUGOUT
//std::ofstream debugOut("TerrainDestructionNetworkingLog.txt");
//#endif 

TerrainDestructionCannBallScene::TerrainDestructionCannBallScene(const std::string& name, SceneManager* sceneMgr,
                                                                 int playerNum)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

    Entity* sqwer = EntityFactory::CreateBmpEntity(*this, d3d, BACKGROUND_TEX, 1000, 1000, m_screenWidth, m_screenHeight, "sqwer");

    // Camera.
    EntityFactory::CreateOrthoFpCameraEntity(*this, -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
		-m_screenHeight / 2.0f, m_screenHeight / 2.0f, "mainCamera");

   /* Texture* mainCircTex = G_TextureManager().GetTexture("cement.dds");
    if(!mainCircTex)
    {
        mainCircTex = G_TextureManager().LoadTexture(d3d, L"cement.dds", "cement.dds");
    }

    Texture* circStencil =  G_TextureManager().GetTexture("circleStencil.dds");
    if(!circStencil)
    {
        circStencil = G_TextureManager().LoadTexture(d3d, L"circleStencil.dds", "circleStencil.dds");
    }*/

    cerr << "test" << endl;
    if(!m_wsa.isOk())
    {
        cerr << "Error initializing wsa." << endl;
    }
    else
    {
        m_networkManager.SetTerrainDestructionCannBallScene(*this);
        m_networkManager.SetPlayerNum(playerNum);
        m_networkManager.start();
    }
}


TerrainDestructionCannBallScene::~TerrainDestructionCannBallScene(void)
{
    m_networkManager.finish();
    m_networkManager.waitForFinish();
}


void TerrainDestructionCannBallScene::Update(double time)
{
    Scene::Update(time);

	//m_physicsSystem.Update(time);
}


void TerrainDestructionCannBallScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}


void TerrainDestructionCannBallScene::AddCircle(float x, float y, float radius, std::string& id)
{
    Entity* newEnt = 
        EntityFactory::CreateBmpEntity(*this, GetParent().GetD3DInstance(), PHYS_CIRC_TEX, 
                                       L"circleStencil.dds", radius*2.0f, radius*2.0f, 
                                       m_screenWidth, m_screenHeight, id);

    newEnt->SetPos(glm::vec3(x, y, 0.0f));
}

void TerrainDestructionCannBallScene::AddSquare(float x, float y, float w, float h, std::string& id)
{
    Entity* newEnt =
        EntityFactory::CreateBmpEntity(*this, GetParent().GetD3DInstance(), PHYS_BLOCK_TEX, 
        w, h, m_screenWidth, m_screenHeight, id);

    newEnt->SetPos(glm::vec3(x, y, 0.0f));
}
