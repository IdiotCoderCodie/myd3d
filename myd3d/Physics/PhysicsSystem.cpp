#include "PhysicsSystem.h"


PhysicsSystem::PhysicsSystem(void) :
m_timer(),
m_dt(0.0f),
m_targetfps(1000), 
m_actualfps(0),
m_parentScene(0),
m_manifold(0),
m_circles(),
m_aabbs(),
m_tweakBar(0)
{
	m_manifold = new ContactManifold();

    m_tweakBar = TwNewBar("Physics Info./Settings");

    TwAddVarRW(m_tweakBar, "targetFPS", TW_TYPE_INT32, &m_targetfps, "");
    TwAddVarRO(m_tweakBar, "actualFPS", TW_TYPE_INT32, &m_actualfps, "");
    TwAddVarRO(m_tweakBar, "dt", TW_TYPE_FLOAT, &m_dt, "step=0.0001");
    TwAddVarRW(m_tweakBar, "GravityVec", TW_TYPE_DIR3F, &m_gravity, "");
    TwAddVarRW(m_tweakBar, "GravityScale", TW_TYPE_FLOAT, &m_gravityScale, "step = 0.01");
}


PhysicsSystem::~PhysicsSystem(void)
{
	/*m_circles.clear();*/

	delete m_manifold;
	m_manifold = 0;
	
	m_parentScene = 0;
}


int PhysicsSystem::run()
{
    SetThreadAffinityMask(this->GetHandle(), 4);
    m_timer.Start();

    float elapsedTime = m_timer.GetTimeInSeconds();
    while (!isFinishing())
    {
        TwRefreshBar(m_tweakBar);
        elapsedTime = m_timer.GetTimeInSeconds();
        if (m_targetfps > 0)
        {
            float targetUpdTime = 1.0f / m_targetfps;
            if (elapsedTime < targetUpdTime)
            {
                float sleepTime = (targetUpdTime - elapsedTime) * 1000.0f;
                DWORD dSleep = (DWORD)sleepTime;
                Sleep((DWORD)sleepTime);
                SetThreadAffinityMask(this->GetHandle(), 4);
                elapsedTime += m_timer.GetTimeInSeconds();
            }
        }
        m_actualfps = (int)(1.0f / elapsedTime);
        Update(elapsedTime);
       // std::cout << elapsedTime << std::endl;
        //Sleep(1);
        SetThreadAffinityMask(this->GetHandle(), 4);
    }

    return 0;
}

void PhysicsSystem::Update(double time)
{
	// TODO: DO IT.
   // time = 1.0f / 60.0f;
	SimulationLoop(time);
}


Circle& PhysicsSystem::AddCircle(Entity* entity, float radius, const glm::vec2& velocity, float mass, float elast)
{
    Circle* circle = new Circle();
	circle->SetParent(entity);
	circle->SetRadius(radius);
	circle->SetVel(velocity.x, velocity.y);
	circle->SetMass(mass);
	circle->SetPos(entity->GetPos().x, entity->GetPos().y);
    circle->SetElasticity(0.6f);

	m_circles.push_back(circle);

    return *circle;
}


AABB& PhysicsSystem::AddAABB(Entity* entity, const glm::vec2& min, const glm::vec2& max, 
                            const glm::vec2& vel, float mass, float elast)
{
    AABB* aabb = new AABB();
    aabb->SetParent(entity);
    aabb->SetMin(min);
    aabb->SetMax(max);
    aabb->SetVel(vel.x, vel.y);
    aabb->SetMass(mass);
    aabb->SetPos(entity->GetPos().x, entity->GetPos().y);
    aabb->SetElasticity(elast);

    m_aabbs.push_back(aabb);

    return *aabb;
}


void PhysicsSystem::SimulationLoop(double time)
{
	m_dt = time;
	// TODO: DO IT.
	// Handle Static collisions.
	//StaticCollisionDetection();

	// Calculate the physics calculations on all objects.

    // Apply gravity to all.
    for (auto& circle : m_circles)
    {
        circle->ApplyForce(glm::vec2(m_gravity.x, m_gravity.y) * m_gravityScale);
    }

    for (auto& aabb : m_aabbs)
    {
        aabb->ApplyForce(glm::vec2(m_gravity.x, m_gravity.y) * m_gravityScale);
    }

	CalculateObjectPhysics();

	// Assess and remove any collisions that no longer take place.
	//m_manifold->Assess();
    m_manifold->Reset();

	// Handle dynamic collisions and update contact manifold for all objects.
	DynamicCollisionDetection();

	// Handle dynamic collision responses using the contact manifold for all objects.
	DynamicCollisionResponse();

	// Update the physics calculations on all objects (e.g. new positions, velocity, etc.)
	UpdateObjectPhysics();
}


void PhysicsSystem::StaticCollisionDetection()
{
	bool loop = true;
	// Do until there are no more static collisions.
	while (loop)
	{
		int checkedNum = 0; // used so we don't check same 2 spheres.
		loop = false;
		for (int circle1 = 0; circle1 < m_circles.size(); circle1++)
		{
			for (int circle2 = checkedNum + 1; circle2 < m_circles.size(); circle2++)
			{
				loop |= StaticSphereCollisionDetection(*m_circles[circle1], *m_circles[circle2]);
			}
			checkedNum++;
		}
	}
}


bool PhysicsSystem::StaticSphereCollisionDetection(Circle& circle1, Circle& circle2)
{
	// Check if the circles are intersecting, if they are, move them out a little.
	bool loop = false;
	glm::vec2 pos1 = circle1.GetPos();
	glm::vec2 pos2 = circle2.GetPos();

	float dist = glm::distance(pos1, pos2) - (circle1.GetRadius() + circle2.GetRadius());
	if (dist < 0.0f)
	{
		glm::vec2 colNormal = glm::normalize(pos1 - pos2);

		pos1 = pos1 - (colNormal * dist * 1.1f);
		circle1.SetPos(pos1.x, pos1.y);

		pos2 = pos2 + (colNormal * dist * 1.1f);
		circle2.SetPos(pos2.x, pos2.y);

		loop = true;
	}

	return loop;
}


void PhysicsSystem::CalculateObjectPhysics()
{
	for (auto& circle : m_circles)
	{
		circle->CalculatePhysics(m_dt);
	}

    for(auto& aabb : m_aabbs)
    {
        aabb->CalculatePhysics(m_dt);
    }
}


void PhysicsSystem::DynamicCollisionDetection()
{
	int checkedNum = 0; // used so we don't check same 2 spheres.
	
	for (int circle1 = 0; circle1 < m_circles.size(); circle1++)
	{
		for (int circle2 = circle1 + 1; circle2 < m_circles.size(); circle2++)
		{
			(*m_circles[circle1]).CollisionWithCircle(*m_circles[circle2], *m_manifold);
		}
		checkedNum++;

        for (int aabb = 0;  aabb < m_aabbs.size(); aabb++)
        {
            m_aabbs[aabb]->CollisionWithCircle(*m_circles[circle1], *m_manifold);
        }
	}

    // AABBvsAABB
    for(int aabb1 = 0; aabb1 < m_aabbs.size(); aabb1++)
    {
        for (int aabb2 = aabb1 + 1; aabb2 < m_aabbs.size(); aabb2++)
        {
            m_aabbs[aabb1]->CollisionWithAABB(*m_aabbs[aabb2], *m_manifold);
        }
    }
}


void PhysicsSystem::DynamicCollisionResponse()
{
	for (int collision = 0; collision < m_manifold->GetNumPoints(); ++collision)
	{
		ManifoldPoint& point = m_manifold->GetPoint(collision);
		if (!point.responded)
		{
			point.contactID1->CollisionResponse(point, m_dt);
		}
	}
}


void PhysicsSystem::UpdateObjectPhysics()
{
	for (auto& circle : m_circles)
	{
		circle->Update();
	}

    for(auto& aabb : m_aabbs)
    {
        aabb->Update();
    }
}
