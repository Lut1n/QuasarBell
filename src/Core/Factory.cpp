#include "Core/Factory.h"

std::unordered_map<std::string, BaseFactory*> BaseFactory::factories;

void BaseFactory::regFactory(const std::string& key, BaseFactory* factory)
{
    factories[key] = factory;
}