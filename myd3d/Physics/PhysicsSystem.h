#pragma once
#include "../Scenes/Scene.h"

class PhysicsSystem
{
public:
    PhysicsSystem(void);
    ~PhysicsSystem(void);

    void Update(double time);

private:
    Scene* m_parentScene;
};

