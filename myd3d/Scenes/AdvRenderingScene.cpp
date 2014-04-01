#include "AdvRenderingScene.h"

#include "../Entities/EntityFactory.h"
#include "../Components/Visual/VisualMeshComponent.h"
#include "../Components/Visual/VisualTessellatedPlanetComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Visual/VisualRaymarchComponent.h"
#include "../Assets/Textures/Texture.h"

#include "SceneManager.h"

AdvRenderingScene::AdvRenderingScene(const std::string& name, SceneManager* sceneMgr)
:	Scene(name, sceneMgr),
	m_renderTarget(0),
	m_renderTarget2(0),
	m_rayMarchTexture1(),
	m_rayMarchTexture2()
{
	D3D& d3d = GetParent().GetD3DInstance();

	int screenWidth = GetParent().GetD3DInstance().GetScreenWidth();
	int screenHeight = GetParent().GetD3DInstance().GetScreenHeight();
	float aspect = screenWidth / (float)screenHeight;

	EntityFactory::CreatePerspectiveFpCameraEntity(*this, 60.0f, aspect, 0.1f, 500.0f, "Camera");

	EntityFactory::CreateAlienMeshEntity(*this, d3d, "Assets\\Models\\teapot.obj", L"cement.dds",
		GetShadowMaps(), glm::vec3(2.8f, 1.6f, -2.6f), glm::vec3(0.02f),
		"Allen");

	EntityFactory::CreateTessellatedTerrainEntity(*this, d3d, "Assets\\Models\\quad4.obj", L"cement.dds",
		L"heightmap3.dds", GetShadowMaps(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 1.0f, 5.0f), "Terrain");

    Entity* torusJesus = EntityFactory::CreateTessellatedTorusEntity(*this, d3d, "Assets\\Models\\quad.obj", L"cement.dds",
		L"noisyHeightmap.dds", GetShadowMaps(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), "TorusJesus");

    torusJesus->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(120.0f, 60.0f, 15.0f)));

     Entity* ellipsoidEnt = EntityFactory::CreateTessellatedEllipsoidEntity(*this, d3d, "Assets\\Models\\quad.obj", L"cement.dds",
		L"noisyHeightmap.dds", GetShadowMaps(), 1.0, 0.65, 0.8, glm::vec3(3.4f, 0.5f, -3.0f), glm::vec3(1.0f), "Ellipsoid");

    // ellipsoidEnt->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(120.0f, 0.0f, 0.0f)));


    EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 1,
         GetShadowMaps(), glm::vec3(5.0f, 0.2f, 5.0f), glm::vec3(0.1f), "SmokeParticles");

    EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 0, 
         GetShadowMaps(), glm::vec3(5.0f, 0.2f, 5.0f), glm::vec3(0.1f), "ExplosionParticles");

	/*EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 1,
		GetShadowMaps(), glm::vec3(5.0f, 0.2f, -5.0f), glm::vec3(0.1f), "SmokeParticles2");

	EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 0,
		GetShadowMaps(), glm::vec3(5.0f, 0.2f, -5.0f), glm::vec3(0.1f), "ExplosionParticles2");*/

	EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 1,
		GetShadowMaps(), glm::vec3(-5.0f, 0.2f, 5.0f), glm::vec3(0.1f), "SmokeParticles3");

	EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 0,
		GetShadowMaps(), glm::vec3(-5.0f, 0.2f, 5.0f), glm::vec3(0.1f), "ExplosionParticles3");

	EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 1,
		GetShadowMaps(), glm::vec3(-5.0f, 0.2f, -5.0f), glm::vec3(0.1f), "SmokeParticles4");

	EntityFactory::CreateGeometryParticleEntity(*this, d3d, "Assets\\Models\\sphere.obj", L"Jan2.dds", 0.05, 0,
		GetShadowMaps(), glm::vec3(-5.0f, 0.2f, -5.0f), glm::vec3(0.1f), "ExplosionParticles4");

	EntityFactory::CreateBezierPatchEntity(*this, d3d, L"nothing.dds", GetShadowMaps(),
										   glm::vec3(0.0f, 2.0f, -3.0f), glm::vec3(1.0f),
		"Warm-Like");

	Entity* light = EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.05f, 0.05f, 0.05f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.5f),
		glm::vec3(2.0f, 10.0f, 0.0f),
		10.0f,
		12.0f,
		"mainLight");	

	/*Entity* light2 = EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.1f, 0.05f, 0.05f, 1.0f),
		glm::vec4(0.5f, 0.1f, 0.1f, 1.0f),
		glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),
		glm::vec3(0.0f, 0.0f, 7.0f),
		10.0f,
		12.0f,
		"mainLight2");*/

 /*   Entity* testRaymarch = new Entity(*this, "testRaymarch");
    testRaymarch->SetComponent(new VisualRaymarchComponent(d3d, "Assets\\Models\\sphere.obj", 3));
    this->AddEntity(testRaymarch);*/

	light->RotateGlobalY(180.0f);

	// Create RayMarch 1.
	Entity* rayMarchEntity = new Entity(*this, "raymarch1");
	rayMarchEntity->SetComponent(new VisualRaymarchComponent(d3d, "Assets\\Models\\sphere.obj", 1));

	m_renderTarget = new RenderTarget(&d3d.GetDevice(), d3d.GetScreenWidth(), d3d.GetScreenHeight());

	m_renderTarget->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);

	m_renderTarget->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());

	rayMarchEntity->Draw(d3d);

	
	m_rayMarchTexture1.SetTexture(m_renderTarget->GetShaderResourceView());

	Entity* rayMesh1 = EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\quad.obj", m_rayMarchTexture1,
		GetShadowMaps(), glm::vec3(5.0f, 2.0f, -3.0f), glm::vec3(2.0f),
		"Raymarch1");

	rayMesh1->RotateGlobalZ(90.0f);
	rayMesh1->RotateGlobalX(90.0f);

	// Create RayMarch 2
	Entity* rayMarchEntity2 = new Entity(*this, "gfdjtdigj");
	rayMarchEntity2->SetComponent(new VisualRaymarchComponent(d3d, "Assets\\Models\\sphere.obj", 0));

	m_renderTarget2 = new RenderTarget(&d3d.GetDevice(), d3d.GetScreenWidth(), d3d.GetScreenHeight());

	m_renderTarget2->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);

	m_renderTarget2->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());

	rayMarchEntity2->Draw(d3d);

	m_rayMarchTexture2.SetTexture(m_renderTarget2->GetShaderResourceView());

	Entity* rayMesh2 = EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\quad.obj", m_rayMarchTexture2,
		GetShadowMaps(), glm::vec3(3.0f, 2.0f, -5.0f), glm::vec3(2.0f),
		"Raymarch2");

	rayMesh2->RotateGlobalX(90.0f);

    // Create RayMarch 3
	Entity* rayMarchEntity3 = new Entity(*this, "gfdjtdigj");
	rayMarchEntity3->SetComponent(new VisualRaymarchComponent(d3d, "Assets\\Models\\sphere.obj", 2));

	m_renderTarget3 = new RenderTarget(&d3d.GetDevice(), d3d.GetScreenWidth(), d3d.GetScreenHeight());

	m_renderTarget3->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);

	m_renderTarget3->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());

	rayMarchEntity3->Draw(d3d);

	m_rayMarchTexture3.SetTexture(m_renderTarget3->GetShaderResourceView());

	Entity* rayMesh3 = EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\quad.obj", m_rayMarchTexture3,
		GetShadowMaps(), glm::vec3(-1.0f, 2.0f, -5.0f), glm::vec3(2.0f),
		"Raymarch2");

	rayMesh3->RotateGlobalX(90.0f);

    // Create RayMarch 4.
	Entity* rayMarchEntity4 = new Entity(*this, "raymarch1");
	rayMarchEntity4->SetComponent(new VisualRaymarchComponent(d3d, "Assets\\Models\\sphere.obj", 3));

	m_renderTarget4 = new RenderTarget(&d3d.GetDevice(), d3d.GetScreenWidth(), d3d.GetScreenHeight());

	m_renderTarget4->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);

	m_renderTarget4->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());

	rayMarchEntity4->Draw(d3d);

	
	m_rayMarchTexture4.SetTexture(m_renderTarget4->GetShaderResourceView());

	Entity* rayMesh4 = EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\quad.obj", m_rayMarchTexture4,
		GetShadowMaps(), glm::vec3(5.0f, 2.0f, 1.0f), glm::vec3(2.0f),
		"Raymarch1");

	rayMesh4->RotateGlobalZ(90.0f);
	rayMesh4->RotateGlobalX(90.0f);
}


AdvRenderingScene::~AdvRenderingScene()
{
}


void AdvRenderingScene::Update(double time)
{
	Scene::Update(time);
}


void AdvRenderingScene::Draw(D3D& d3d)
{
	for (int i = 0; i < GetShadowMaps().size(); i++)
	{
		GetShadowMaps()[i]->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(),
			1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Do shadow pass.
	for (auto ent : GetEntities())
	{
		Component* vmcCheck = ent->GetComponent("VisualComponent", "VisualMeshComponent");
		if (vmcCheck)
		{
			// entity has a VMC.
			VisualMeshComponent* vmc = static_cast<VisualMeshComponent*>(vmcCheck);
			//vmc->ShadowPass(d3d);
		}
	}

	d3d.BeginScene(0.3f, 0.3f, 0.3f, 1.0f);

	d3d.SetBackBufferRenderTarget();

	Scene::Draw(d3d);
}