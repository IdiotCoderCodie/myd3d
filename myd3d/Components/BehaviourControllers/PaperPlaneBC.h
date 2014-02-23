#pragma once
#include "../Component.h"
#include "../Collision/CollisionComponent.h"
#include "../Visual/ParticleSystemComponent.h"
#include "../Physics/FollowPathComponent.h"

class PaperPlaneBC : public Component
{
public:
    explicit PaperPlaneBC(D3D& d3d);
    ~PaperPlaneBC(void);

    void FamilyID(componentId_t& idOut) const;
    void ComponentID(componentId_t& idOut) const;

    void Update(float time);

    void Draw(D3D& d3d);

private:
    void UpdateFire(float time);

private:
    bool                     m_onFire;
    float                    m_fireTime;
    ParticleSystemComponent  m_fireSystem;
};
