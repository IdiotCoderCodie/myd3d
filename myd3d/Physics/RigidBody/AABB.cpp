#include "AABB.h"

AABB::AABB()
{
}


AABB::~AABB()
{
}



void AABB::CollisionWithCircle(Circle& circle, ContactManifold& manifold)
{
}


void AABB::CollisionWithAABB(AABB& aabb, ContactManifold& contactManifold)
{

}


void AABB::CollisionWithPolygon(RBPolygon& polygon, ContactManifold& manifold)
{

}


void AABB::CollisionResponse(ManifoldPoint& point)
{

}

//void AABB::CollisionResponseWithCircle(ManifoldPoint& point)
//{
//
//}
//
//
//void AABB::CollisionResponseWithAABB(ManifoldPoint& point)
//{
//
//}
//
//
//void AABB::CollisionResponseWithPolygon(ManifoldPoint* point)
//{
//
//}