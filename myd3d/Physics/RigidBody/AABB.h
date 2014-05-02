#pragma once
#include "RigidBody.h"

class AABB : public RigidBody
{
public:
    AABB();
    ~AABB();

    int GetType() { return RB_TYPE_AABB; }

    glm::vec2& GetMin() { return m_min; }
    glm::vec2& GetMax() { return m_max; }

    void SetMin(const glm::vec2& min) { m_min = min; }
    void SetMax(const glm::vec2& max) { m_max = max; }

    void CollisionWithCircle(Circle& circle, ContactManifold& manifold);
    void CollisionWithAABB(AABB& aabb, ContactManifold& contactManifold);
    void CollisionWithPolygon(RBPolygon& polygon, ContactManifold& manifold);

private:
    glm::vec2 m_min;
    glm::vec2 m_max;
};

