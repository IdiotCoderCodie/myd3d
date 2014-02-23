#include "PaperPlaneBC.h"
#include "../../Entities/Entity.h"
#include "../../InputManager.h"

PaperPlaneBC::PaperPlaneBC(D3D& d3d)
    :   m_onFire(false),             
        m_fireTime(0.0f),
        m_fireSystem(d3d, "flameParticleEffect.txt")
{
}


PaperPlaneBC::~PaperPlaneBC(void)
{
}


void PaperPlaneBC::FamilyID(componentId_t& idOut) const    
{ 
    idOut = "PaperPlaneBC"; 
}

void PaperPlaneBC::ComponentID(componentId_t& idOut) const 
{ 
    idOut = "PaperPlaneBC"; 
}


void PaperPlaneBC::Update(float time)
{
    // Ensure the particle system's parent is set as the same as this components before updating.
    m_fireSystem.SetParent(&this->GetParent());
    m_fireSystem.Update(time);

    UpdateFire(time);   
}

void PaperPlaneBC::UpdateFire(float time)
{
    FollowPathComponent* pathComp = 
                static_cast<FollowPathComponent*>(GetParent().GetComponent("PathComponent"));

    // Check to reset.
    if(G_InputManager().IsKeyPressed(DIK_R))
    {
        if(pathComp)
        {
            pathComp->Restart();
            m_onFire = false;
            m_fireSystem.HardStop();
            m_fireTime = 0.0f;
        }
    }

    if(!m_onFire)
    {
        CollisionComponent* collisionComp = 
            static_cast<CollisionComponent*>(GetParent().GetComponent("CollisionComponent"));
        if(collisionComp)
        {
            if(collisionComp->IsColliding())
            {
                m_fireSystem.Start();
                m_onFire = true;              
            }
        }
    }
    else
    {
        // Currently on fire. 
        m_fireTime += time;
        if(m_fireTime > 5.0f)
        {
            // Turn fire off.
            m_fireTime = 0.0f;
            m_onFire = false;
            m_fireSystem.HardStop();
           
            if(pathComp)
            {
                pathComp->Restart();
            }
        }
    }   
}

void PaperPlaneBC::Draw(D3D& d3d)
{
    m_fireSystem.Draw(d3d);
}
