#include "json.hpp"

#include <algorithm> // std::find

//--------------------------------------------------------------
JsonValue JsonValue::s_nullVal;

//--------------------------------------------------------------
JsonValue::JsonValue()
{
}
//--------------------------------------------------------------
JsonValue::~JsonValue()
{
}
//--------------------------------------------------------------
JsonValue::KeyIdVector::iterator JsonValue::ArrMap::find(const std::string& k)
{
    return std::find_if(keys_indexes.begin(), keys_indexes.end(), [k](const KeyId& pr){return pr.first==k;});
}
//--------------------------------------------------------------
JsonValue::KeyIdVector::const_iterator JsonValue::ArrMap::find(const std::string& k) const
{
    return std::find_if(keys_indexes.begin(), keys_indexes.end(), [k](const KeyId& pr){return pr.first==k;});
}
//--------------------------------------------------------------
size_t JsonValue::ArrMap::at(const std::string& k) const
{
    auto it = find(k);
    if(it != keys_indexes.end())
        return it->second;
    return 0;
}
//--------------------------------------------------------------
void JsonValue::ArrMap::add(const std::string& k, const JsonValue& v)
{
    keys_indexes.push_back(std::make_pair(k, values.size()));
    values.emplace_back(v);
}
//--------------------------------------------------------------
bool JsonValue::has(int i) const
{
    return i>=0 && i<array.values.size();
}
//--------------------------------------------------------------
bool JsonValue::has(const std::string& key) const
{
    return array.find(key) != array.keys_indexes.end();
}
//--------------------------------------------------------------
JsonValue::Type JsonValue::typeAt(int i) const
{
    return operator[](i).type;
}
//--------------------------------------------------------------
JsonValue::Type JsonValue::typeAt(const std::string& key) const
{
    return operator[](key).type;
}
//--------------------------------------------------------------
const JsonValue& JsonValue::operator[](int i) const
{
    if(type != Type::Array)
    {
        std::cout << "value is not an json array" << std::endl;
        return s_nullVal;
    }
    if(has(i))
    {
        return array.values[i];
    }
    return s_nullVal;
}
//--------------------------------------------------------------
const JsonValue& JsonValue::operator[](const std::string& key) const
{
    if(type != Type::Object)
    {
        std::cout << "value is not an json object" << std::endl;
        return s_nullVal;
    }
    if(has(key))
    {
        int i = array.at(key);
        return array.values[i];
    }
    return s_nullVal;
}
//--------------------------------------------------------------
bool JsonValue::getBoolean() const
{
    if(type != Type::Boolean)
    {
        std::cout << "json value is not boolean" << std::endl;
        return false;
    }
    return boolean;
}
//--------------------------------------------------------------
double JsonValue::getNumeric() const
{
    if(type != Type::Numeric)
    {
        std::cout << "json value is not numeric" << std::endl;
        return 0.0;
    }
    return numeric;
}
//--------------------------------------------------------------
const std::string& JsonValue::getString() const
{
    if(type != Type::String)
    {
        std::cout << "json value is not string" << std::endl;
        return s_nullVal.string;
    }
    return string;
}
//--------------------------------------------------------------
void JsonValue::clear()
{
    array.values.clear();
    array.keys_indexes.clear();
    type = Type::Undefined;
}
//--------------------------------------------------------------
void JsonValue::set(bool b)
{
    array.values.clear();
    array.keys_indexes.clear();
    type = Type::Boolean;
    boolean = b;
}
//--------------------------------------------------------------
void JsonValue::set(double d)
{
    array.values.clear();
    array.keys_indexes.clear();
    type = Type::Numeric;
    numeric = d;
}
//--------------------------------------------------------------
void JsonValue::set(const std::string& s)
{
    array.values.clear();
    array.keys_indexes.clear();
    type = Type::String;
    string = s;
}
//--------------------------------------------------------------
JsonValue& JsonValue::setEntry(int i)
{
    if(type != Type::Array)
    {
        array.values.clear();
        array.keys_indexes.clear();
    }
    type = Type::Array;
    if(!has(i))
        array.values.resize(i+1);
    
    return array.values[i];
}
//--------------------------------------------------------------
JsonValue& JsonValue::setEntry(const std::string& key)
{
    if(type != Type::Object)
    {
        array.values.clear();
        array.keys_indexes.clear();
    }
    type = Type::Object;
    
    if(!has(key))
        array.add(key, JsonValue());
    
    return array.values[ array.at(key) ];
}
