#pragma once
#include "Scene.h"

class Example2DScene : public Scene
{
public:
	Example2DScene(const std::string& name, SceneManager* sceneMgr);
	~Example2DScene();

	void Update(double time);
	void Draw(D3D& d3d);

private:
	int m_screenWidth;
	int m_screenHeight;
};

