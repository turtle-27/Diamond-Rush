#include "ECS.hpp"

void Entity::addGroup(Group mGroup)
{
    groupBitset[mGroup] = true;
    manager.AddtoGroup(this, mGroup);
}