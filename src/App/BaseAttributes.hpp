#ifndef GUI_BASE_PROPERTIES_H
#define GUI_BASE_PROPERTIES_H

#include "Json/Json.hpp"

#include <vector>
#include <string>
#include <tuple>

//--------------------------------------------------------------
// forward
class JsonValue;

//--------------------------------------------------------------
struct BaseAttributes
{
    enum class Type
    {
        Undefined,
        Error,
        Bool,
        Int,
        Float,
        Color3
    };

    enum class IoType
    {
        Undefined = 0,
        Error = 1,
        Signal = 1 << 1,
        Image = 1 << 2,
        Geometry = 1 << 3,
        ImageGeometry = Image | Geometry
    };
    
    union MinMaxVal
    {
        float f2[2];
        int i2[2];
    };

    using Wrapper = std::tuple<std::string,Type,void*,MinMaxVal>;

    std::vector<Wrapper> wrappers;
    std::vector<std::pair<std::string, IoType>> inputs;
    std::vector<std::pair<std::string, IoType>> outputs;
    bool _hasCustomData = false;
    bool _hasChanged = true;
    bool _connectionChanged = true;

    size_t typeId = 0;

    BaseAttributes(size_t typeId) : typeId(typeId) {}

    virtual ~BaseAttributes() = default;
    
    size_t count() const;
    Type typeAt(size_t i) const;
    std::string nameAt(size_t i) const;
    
    size_t inputCount() const;
    IoType inputType(size_t i) const;
    std::string inputName(size_t i) const;
    
    size_t outputCount() const;
    IoType outputType(size_t i) const;
    std::string outputName(size_t i) const;
    
    void get(size_t i, std::string& value) const;
    void get(size_t i, float& value) const;
    void get(size_t i, int& value) const;
    void get(size_t i, float* value) const;
    void getMinMax(size_t i, float& minVal, float& maxVal) const;
    void getMinMax(size_t i, int& minVal, int& maxVal) const;
    void get(size_t i, bool& value) const;
    void set(size_t i, const std::string& value);
    void set(size_t i, float value);
    void set(size_t i, int value);
    void set(size_t i, bool value);
    void set(size_t i, const float* value);
    
    void add(const std::string& name, Type type, void* ptr);
    void add(const std::string& name, float* ptr, float minVal, float maxVal);
    void add(const std::string& name, int* ptr, int minVal, int maxVal);
    void addInput(const std::string& name, IoType type);
    void addOutput(const std::string& name, IoType type);

    void ui(int index);
    virtual void ui();
    
    bool hasCustomData() const;
    virtual void saveCustomData(JsonValue& json);
    virtual void loadCustomData(JsonValue& json);
    
    template<typename Ty>
    static Ty& as(Wrapper& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
    template<typename Ty>
    static const Ty& as(const Wrapper& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
    
    void onChanged()
    {
        _hasChanged = true;
    }
    
    void onConnectionChanged()
    {
        _connectionChanged = true;
    }

    bool hasChanged() const
    {
        return _hasChanged;
    }

    bool connectionChanged() const
    {
        return _connectionChanged;
    }

    void validate()
    {
        _hasChanged = false;
        _connectionChanged = false;
    }
};

//--------------------------------------------------------------
template<typename T>
struct Tree
{
    std::vector<Tree> children;
    T value;

    T& get() {return value;}
    const T& get() const {return value;}
    void set(const T& v) {value = v;}
    void set(T& v) {value = v;}

    void reserve(size_t i)
    {
        children.resize(i);
    }

    Tree& at(size_t i)
    {
        if (i >= children.size())
            children.resize(i+1);

        return children[i];
    }

    const Tree& at(size_t i) const
    {
        if (i >= children.size())
            children.resize(i+1);

        return children[i];
    }
};

#endif // GUI_BASE_PROPERTIES_H
