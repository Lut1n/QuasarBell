#ifndef QB_FACTORY_H
#define QB_FACTORY_H

#include <array>
#include <cstddef> // size_t

struct BaseFactory
{
    BaseFactory() = default;
    virtual ~BaseFactory() = default;

    void regFactory(size_t typeId, BaseFactory* factory);
    
    static std::array<BaseFactory*, 256> factories;
};

template<typename Base>
struct Factory : public BaseFactory
{
    virtual Base* create() = 0;

    static Base* create(size_t typeId)
    {
        if (typeId < factories.size() && factories[typeId] != nullptr)
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