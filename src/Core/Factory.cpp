#include "core/Factory.h"

std::unordered_map<size_t, BaseFactory*> BaseFactory::factories;

void BaseFactory::regFactory(size_t typeId, BaseFactory* factory)
{
    factories[typeId] = factory;
}