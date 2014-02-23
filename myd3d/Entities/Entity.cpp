#include <cassert>

#include "Entity.h"
#include <AntTweakBar.h>


Entity::Entity(Scene& parent, const entityId_t& id)
    :   m_Parent(parent),
        m_id(id),
        m_Transform(),
        m_Components(),
        m_tweakBar(0)
{
    // Create tweak bar.
    m_tweakBar = TwNewBar(id.c_str());
    
    // Add various transform variables to the tweak bar.
    TwAddVarRO(m_tweakBar, "PosX", TW_TYPE_FLOAT, &m_Transform.GetPosition().x, 
        " label='X' group='Position' readonly='false' step=0.1");
    TwAddVarRO(m_tweakBar, "PosY", TW_TYPE_FLOAT, &m_Transform.GetPosition().y, 
        " label='Y' group='Position' readonly='false' step=0.1");
    TwAddVarRO(m_tweakBar, "PosZ", TW_TYPE_FLOAT, &m_Transform.GetPosition().z, 
        " label='Z' group='Position' readonly='false' step=0.1");
    // Put the "Position" group into the Transform group.
    std::string extraStr = "/Position group='Transform'";
    TwDefine((id + extraStr).c_str());

    TwAddVarRO(m_tweakBar, "Forward", TW_TYPE_DIR3F, &m_Transform.GetForward(),
        " group='Transform' ");
    TwAddVarRO(m_tweakBar, "Up", TW_TYPE_DIR3F, &m_Transform.GetUp(),
        " group='Transform' ");

    TwAddVarRO(m_tweakBar, "Scale", TW_TYPE_DIR3F, &m_Transform.GetScale(), 
        "group='Transform' readonly='false' step=0.1");

    // Iconify the tweak bar.
    TwDefine((id + " iconified=true ").c_str());
}


Entity::Entity(Scene& parent, const entityId_t& id, Frame& transform)
    :   m_Parent(parent),
        m_id(id),
        m_Transform(transform),
        m_Components()
{
    m_tweakBar = TwNewBar(id.c_str());
    TwAddVarRO(m_tweakBar, id.c_str(), TW_TYPE_DIR3F, &m_Transform.GetPosition(), "");
}


Entity::Entity(Scene& parent, const entityId_t& id, const glm::vec3& pos, const glm::vec3& forward, 
               const glm::vec3& up)
    :   m_Parent(parent),
        m_id(id),
        m_Transform(pos, forward, up),
        m_Components()
{
    m_tweakBar = TwNewBar(id.c_str());
    TwAddVarRO(m_tweakBar, id.c_str(), TW_TYPE_DIR3F, &m_Transform.GetPosition(), "");
}


Entity::~Entity(void)
{
    try
    {
        ClearComponents();
    }
    catch (int& e)
    {
        // Exception thrown trying to clear components.
        assert(false);
    }
}

 Entity& Entity::operator=(const Entity& other)
 {
     m_id = other.m_id;
     m_Transform = other.m_Transform;
     m_Components = other.m_Components;
     m_tweakBar = 0;
     //m_Parent;
     return *this;
 }


Component* Entity::GetComponent(const componentId_t& familyId)
{
    if(m_Components.find(familyId) != m_Components.end())
    {
        return m_Components.find(familyId)->second;
    }
    return 0;
}

Component* Entity::GetComponent(const componentId_t& familyId, const componentId_t& componentId)
{
    if(m_Components.find(familyId) != m_Components.end())
    {
        std::string compId;
        m_Components.find(familyId)->second->ComponentID(compId);
        if(compId.compare(componentId) == 0)
        {
            // This is a component of the asks type.
            return m_Components.find(familyId)->second;
        }
    }

    return 0;
}


void Entity::SetComponent(Component* newComponent)
{
    componentId_t gocId;
    newComponent->FamilyID(gocId);

    Component* temp = m_Components[gocId];
    // Discard of this old component.
    if(temp)
        delete temp;
    temp = 0;

    // Add the component, setting it's parent.
    newComponent->SetParent(this);
    m_Components[gocId] = newComponent;
}


void Entity::ClearComponents()
{   // Iterate through all components and delete them.
    for(auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        delete it->second;
        it->second = 0;
    }
    // Now clear the m_Components map.
    m_Components.erase(m_Components.begin(), m_Components.end());
}


void Entity::Update(double time)
{
    for(auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        it->second->Update(time);
    }
}


void Entity::Draw(D3D& d3d)
{
    for(auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        it->second->Draw(d3d);
    }
}
