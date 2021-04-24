#ifndef QB_FACTORY_H
#define QB_FACTORY_H

#include <unordered_map>
#include <string>

struct BaseFactory
{
    BaseFactory() = default;
    virtual ~BaseFactory() = default;

    void regFactory(size_t typeId, BaseFactory* factory);
    
    static std::unordered_map<size_t, BaseFactory*> factories;
};

template<typename Base>
struct Factory : public BaseFactory
{
    virtual Base* create() = 0;

    static Base* create(size_t typeId)
    {
        if (factories.find(typeId) != factories.end())
        {
            return static_cast<Factory<Base>*>(factories[typeId])->create();
        }
        return nullptr;
    }
};

template<typename Base, typename Type>
struct TypedFactory : public Factory<Base>
{
    TypedFactory(size_t typeId)
    {
        this->regFactory(typeId, this);
    }
    Base* create() override
    {
        return new Type();
    }
};

#endif // QB_FACTORY_H