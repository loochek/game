#include "PhysicObject.h"

PhysicObject::PhysicObject(TiledMap *map, AABB aabb) : aabb(aabb)
{
	this->map = map;
}

PhysicObject::~PhysicObject()
{
}

void PhysicObject::updatePhysics(sf::Time delta)
{
	position += speed * delta.asSeconds();
}