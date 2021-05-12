#ifndef QB_FACTORY_H
#define QB_FACTORY_H

#include <array>
#include <cstddef> // size_t

//--------------------------------------------------------------
template<typename Base>
struct FactoryCreator
{
    FactoryCreator() = default;
    virtual ~FactoryCreator() = default;
    virtual Base* create() = 0;
};
//--------------------------------------------------------------
template<typename Base>
struct Factory
{
    static void regCreator(size_t typeId, FactoryCreator<Base>* creator)
    {
        creators[typeId] = creator;
    }
    static Base* create(size_t typeId)
    {
        if (typeId < creators.size() && creators[typeId] != nullptr)
        {
            return static_cast<Base*>(creators[typeId]->create());
        }
        return nullptr;
    }
    static std::array<FactoryCreator<Base>*, 256> creators;
};
template<typename Base>
std::array<FactoryCreator<Base>*, 256> Factory<Base>::creators = {};
//--------------------------------------------------------------
template<typename Base, typename Type>
struct TypedCreator : public FactoryCreator<Base>
{
    TypedCreator(size_t typeId)
    {
        Factory<Base>::regCreator(typeId, this);
    }
    Base* create() override
    {
        return new Type();
    }
};

#endif // QB_FACTORY_H