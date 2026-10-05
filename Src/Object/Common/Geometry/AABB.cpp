#include "AABB.h"

AABB::AABB(VECTOR& min, VECTOR& max):min_(min),max_(max)
{
}

AABB::~AABB(void)
{
}

void AABB::Update(void)
{
}

void AABB::Draw(void)
{
}

const bool AABB::IsHit(Geometry& _geometry, VECTOR& _hitPos)
{
	return false;
}

const bool AABB::IsHit(Sphere& _sphere, VECTOR& _hitPos)
{
	return false;
}

const bool AABB::IsHit(Capsule& _capsule, VECTOR& _hitPos)
{
	return false;
}

const bool AABB::IsHit(Line3D& _line, VECTOR& _hitPos)
{
	return false;
}

const bool AABB::IsHit(Triangle3D& _triangle, VECTOR& _hitPos)
{
	return false;
}

const bool AABB::IsHit(Cylinder& _circle, VECTOR& _hitPos)
{
	return false;
}

const bool AABB::IsHit(AABB& _box, VECTOR& _hitPos)
{
	return false;
}
