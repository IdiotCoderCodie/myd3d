#pragma once
#include "RigidBody.h"
#include <vector>

class RBPolygon : public RigidBody
{
public:
    RBPolygon(void);
    RBPolygon(std::vector<glm::vec2>& vertices);
    ~RBPolygon(void);

    glm::vec2& operator[](unsigned int i) { return m_vertices[i]; }
    const glm::vec2& operator[](unsigned int i) const { return m_vertices[i]; }

    std::vector<glm::vec2> const& GetVertices() { return m_vertices; }
    int GetNumVertices() const { return m_vertices.size(); }

    void CollisionWithCircle(Circle* circle, ContactManifold* ContactManifold);
    void CollisionWithPolygon(RBPolygon* polygon, ContactManifold* ContactManifold);

    static bool Intersect(RBPolygon& A, RBPolygon& B, glm::vec2& MTD);  
    static bool AxisSeparatePolygons(const glm::vec2& axis, const RBPolygon& A, const RBPolygon& B);
    static void CalculateInterval(const glm::vec2& axis, const RBPolygon& P, float& min, float& max);
    static glm::vec2 FindMTD(std::vector<glm::vec2>& Axes);

private:
    std::vector<glm::vec2> m_vertices;
};

