#pragma once
#include "Scene.h"
#include "../RenderTarget.h"
#include "../Assets/Textures/Texture.h"

class AdvRenderingScene : public Scene
{
public:
	AdvRenderingScene(const std::string& name, SceneManager* sceneMgr);
	~AdvRenderingScene();

	void Draw(D3D& d3d);
	void Update(double time);

private:
	// HACK: Using render target to render raymarch to, then using Texture to hold the
	// shader resource view which is passed through to the model.
	RenderTarget* m_renderTarget;
	Texture m_meshTexture1;
};

