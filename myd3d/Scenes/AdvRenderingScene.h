#pragma once
#include "Scene.h"

class AdvRenderingScene : public Scene
{
public:
	AdvRenderingScene(const std::string& name, SceneManager* sceneMgr);
	~AdvRenderingScene();

	void Draw(D3D& d3d);
	void Update(double time);

	
};

