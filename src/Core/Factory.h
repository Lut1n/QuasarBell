#ifndef QB_FACTORY_H
#define QB_FACTORY_H

#include <unordered_map>
#include <string>

struct BaseFactory
{
    BaseFactory() = default;
    virtual ~BaseFactory() = default;

    void regFactory(const std::string& key, BaseFactory* factory);
    
    static std::unordered_map<std::string, BaseFactory*> factories;
};

template<typename Base>
struct Factory : public BaseFactory
{
    virtual Base* create() = 0;

    static Base* create(const std::string& key)
    {
        if (factories.find(key) != factories.end())
        {
            return static_cast<Factory<Base>*>(factories[key])->create();
        }
        return nullptr;
    }
};

template<typename Base, typename Type>
struct TypedFactory : public Factory<Base>
{
    TypedFactory(const std::string& key)
    {
        this->regFactory(key, this);
    }
    Base* create() override
    {
        return new Type();
    }
};

#endif // QB_FACTORY_H