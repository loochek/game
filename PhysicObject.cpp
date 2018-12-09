#include "PhysicObject.h"

PhysicObject::PhysicObject(TiledMap *map)
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