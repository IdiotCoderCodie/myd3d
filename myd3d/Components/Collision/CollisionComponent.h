#pragma once
#include "../Component.h"

enum CollisionType 
{
    BoundingBox,
    BoundingSphere,
    Mesh
};

struct CollisionComponentDesc
{
    CollisionType collisionType;
    float         radius;
    float         width;
    float         height;
    float         depth;
};

class CollisionComponent : public Component
{
public:
    explicit CollisionComponent(const CollisionComponentDesc& desc);
    ~CollisionComponent(void);

    void FamilyID(componentId_t& id) const;
    virtual void ComponentID(componentId_t& id) const;

    virtual void Update(float time);

    void SetCollisionComponentDesc(const CollisionComponentDesc& desc) { m_desc = desc; }
    const CollisionComponentDesc& GetCollisionComponentDesc() const { return m_desc; }

    bool IsColliding() const          { return m_colliding; }
    void SetColliding(bool colliding) { m_colliding = colliding; }

    bool CheckCollision(const CollisionComponent* other);

private:
    CollisionComponentDesc  m_desc;
    bool                    m_colliding;
    CollisionComponent*     m_collidingWith;
};

