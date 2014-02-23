#pragma once
#include "Scene.h"

class ExampleScene : public Scene
{
public:
	ExampleScene(const std::string& name, SceneManager* sceneMgr);
	~ExampleScene();

	void Draw(D3D& d3d);
	void Update(double time);

private:

};

