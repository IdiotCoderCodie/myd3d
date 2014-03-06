#pragma once
#include "../glm/glm.hpp"

class Circle;

struct ManifoldPoint 
{
    glm::vec2 contactPoint;
    Circle* contactID1;
    Circle* contactID2;
    glm::vec2 contactNormal;
    bool responded;
};

class ContactManifold
{
public:
    ContactManifold(void);
    ~ContactManifold(void);

    void Add(ManifoldPoint point);
    void Assess();
	int GetNumPoints() const			{ return m_numOfPoints; }
	ManifoldPoint& GetPoint(int index)	{ return m_points[index]; }

private:
    ManifoldPoint m_points[1000];
    int m_numOfPoints;
};

