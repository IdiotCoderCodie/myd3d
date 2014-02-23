#include "CollisionComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"

CollisionComponent::CollisionComponent(const CollisionComponentDesc& desc)
    : m_desc(desc),
      m_colliding(false),
      m_collidingWith(0)
{
}


CollisionComponent::~CollisionComponent(void)
{
}


void CollisionComponent::FamilyID(componentId_t& id) const            
{ 
    id = "CollisionComponent"; 
}

void CollisionComponent::ComponentID(componentId_t& id) const 
{ 
    id = "BasicCollisionComponent";
}


void CollisionComponent::Update(float time)
{
    m_colliding = false;
    auto& entities = GetParent().GetParent().GetEntities();
    for(Entity* e : entities)
    {
        CollisionComponent* other = (CollisionComponent*)e->GetComponent("CollisionComponent");
        if(other)
        {
            CheckCollision(other);
        }
    }
}


bool CollisionComponent::CheckCollision(const CollisionComponent* other)
{
    if(this == other)
    {
        return false;
    }

    const CollisionComponentDesc& otherDesc = other->GetCollisionComponentDesc();
    if(m_desc.collisionType == CollisionType::BoundingSphere)
    {
        if(otherDesc.collisionType == CollisionType::BoundingSphere)
        {
            // Do sphere->sphere collision check.
            float radii = m_desc.radius + otherDesc.radius;
            if(glm::distance(other->GetParent().GetPos(), this->GetParent().GetPos()) < radii)
            {
                // Collision between bounding spheres!
                m_colliding = true;
                // NOTE: Call something here to react to this collision. 
            }
        }
        else if(otherDesc.collisionType == CollisionType::BoundingBox)
        {
            // DO SPHERE->BOUNDING BOX COLLISION CHECK
        }
        else
        {
            // DO SPHERE->MESH COLLISION CHECK
        }
    }

    return m_colliding;
}
