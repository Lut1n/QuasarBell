#ifndef JSON_MODEL_H
#define JSON_MODEL_H

#include <iostream>
#include <vector>

//-------------------------------------------------------
class JsonValue
{
protected:
    static JsonValue s_nullVal;
public:
    JsonValue();
    virtual ~JsonValue();
    
    using KeyId = std::pair<std::string, size_t>;
    using KeyIdVector = std::vector<KeyId>;
    
    struct ArrMap
    {
        std::vector<JsonValue> values;
        KeyIdVector keys_indexes;
        
        KeyIdVector::iterator find(const std::string& k);
        KeyIdVector::const_iterator find(const std::string& k) const;
        size_t at(const std::string& k) const;
        void add(const std::string& k, const JsonValue& v);
    };
    
    enum class Type
    {
        Undefined,
        Array,
        Object,
        Boolean,
        Numeric,
        String
    };
    
    ArrMap array;
    bool boolean = false;
    double numeric = 0.0;
    std::string string;
    
    Type type = Type::Undefined;

    bool has(int i) const;
    bool has(const std::string& key) const;
    
    Type typeAt(int i) const;
    Type typeAt(const std::string& key) const;
    
    const JsonValue& operator[](int i) const;
    const JsonValue& operator[](const std::string& key) const;
    
    bool getBoolean() const;
    double getNumeric() const;
    const std::string& getString() const;
    
    // edition
    void clear();
    void set(bool b);
    void set(double d);
    void set(const std::string& s);
    JsonValue& setEntry(int i);
    JsonValue& setEntry(const std::string& key);
    
    template <typename T, typename... Types> 
    JsonValue& setPath(T var1, Types... var2) 
    {
        return setEntry(var1).setPath(var2...);
    }
    JsonValue& setPath() 
    {
        return *this;
    }
    
};

#endif // JSON_MODEL_H
