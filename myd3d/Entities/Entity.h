#pragma once
#include <string>
#include <map> 

#include <AntTweakBar.h>

#include "../glm/glm.hpp"
#include "../Math/Frame.h"
#include "../Components/Component.h"

class Scene;

typedef std::string entityId_t;

using namespace glm;

class Entity
{
public:
    Entity(Scene& parent, const entityId_t& id);
    Entity(Scene& parent, const entityId_t& id, Frame& transform);
    Entity(Scene& parent, const entityId_t& id, const glm::vec3& pos, const glm::vec3& forward, 
        const glm::vec3& up);
    ~Entity();
private:
    Entity& operator=(const Entity& other);
public:

    const Scene& GetParent() const { return m_Parent; }
    Scene& GetParent() { return m_Parent; }

    const entityId_t GetID() const              { return m_id; }
    void SetID(const entityId_t& id)             { m_id = id; }

    Component* GetComponent(const componentId_t& familyId) ;
    Component* GetComponent(const componentId_t& familyId, const componentId_t& componentId);
    void SetComponent(Component* newComponent);
    void ClearComponents();

    void Update(double time);

    void Draw(D3D& d3d);


    //----------------------------------------------------------------------------------------------
    // Helper functions for moving/rotating the entity
    //----------------------------------------------------------------------------------------------

    const Frame& GetTransform() const           { return m_Transform; }
    void SetTransform(const Frame& transform)   { m_Transform = transform; }

    const glm::vec3& GetPos() const             { return m_Transform.GetPosition(); }
    void SetPos(const glm::vec3& newPos)        { m_Transform.SetPosition(newPos); }

    void MoveForward(float delta)               { m_Transform.MoveForward(delta) ; }
    void MoveUp(float delta)                    { m_Transform.MoveUp(delta); }
    void MoveRight(float delta)                 { m_Transform.MoveRight(delta); }
    void MoveGlobalX(float delta)               { m_Transform.MoveGlobalX(delta); }
    void MoveGlobalY(float delta)               { m_Transform.MoveGlobalY(delta); }
    void MoveGlobalZ(float delta)               { m_Transform.MoveGlobalZ(delta); }

    void RotateLocalX(float delta)              { m_Transform.RotateLocalX(delta); }
    void RotateLocalY(float delta)              { m_Transform.RotateLocalY(delta); }
    void RotateLocalZ(float delta)              { m_Transform.RotateLocalZ(delta); }

    void RotateGlobalX(float delta)             { m_Transform.RotateGlobalX(delta); }
    void RotateGlobalY(float delta)             { m_Transform.RotateGlobalY(delta); }
    void RotateGlobalZ(float delta)             { m_Transform.RotateGlobalZ(delta); }

    void SetScaleX(float x)                        { m_Transform.SetScaleX(x); }
    void SetScaleY(float y)                        { m_Transform.SetScaleY(y); }
    void SetScaleZ(float z)                        { m_Transform.SetScaleZ(z); }

    TwBar* GetTweakBar() const { return m_tweakBar; }

private:
    Scene&                              m_Parent;
    entityId_t                          m_id;
    Frame                               m_Transform;
    std::map<componentId_t, Component*> m_Components;
    TwBar*                              m_tweakBar;
};
