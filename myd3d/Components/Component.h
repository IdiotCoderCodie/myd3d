#pragma once

#include <string>

class Entity;
class D3D;

typedef std::string componentId_t;

class Component
{
public:
    Component();
    virtual ~Component() = 0;
    
    // Sets param "out" to the FamilyID of the component.
    virtual void FamilyID(componentId_t& out) const = 0;
    // Sets param "out" to the ComponentID of the component.
    virtual void ComponentID(componentId_t& out) const = 0;

    // Param "time" signifies time elapsed since the last update call.
    virtual void Update(float time);

    virtual void Draw(D3D& d3d);

    void SetParent(Entity* ent)     { m_Parent = &(*ent); }
    Entity& GetParent() const { return *m_Parent; }

private:
    Entity* m_Parent;

};
