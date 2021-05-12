#include "Core/Factory.h"

std::array<BaseFactory*, 256> BaseFactory::factories = {};

void BaseFactory::regFactory(size_t typeId, BaseFactory* factory)
{
    factories[typeId] = factory;
}