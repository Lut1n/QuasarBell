#include "JsonReaderWriter.hpp"

#include <sstream>

//--------------------------------------------------------------
int getEndOf(const std::string& str, int beginPos);

//--------------------------------------------------------------
std::string substringPP(const std::string& str, int p1, int p2)
{
	int len = p2-p1+1;
	return str.substr(p1, len);
}
//--------------------------------------------------------------
std::string substring(const std::string& str, int pos, int len)
{
	return str.substr(pos, len);
}
//--------------------------------------------------------------
double asDouble(const std::string& str)
{
	std::stringstream ss(str);
	double d;
	ss >> d;
	return d;
}
//--------------------------------------------------------------
bool asBoolean(const std::string& str)
{
	return str=="true" || str=="True" || str=="TRUE";
}
//--------------------------------------------------------------
int first(const std::string& str, char c)
{
	for(int i=0; i<(int)str.size(); ++i)
	{
		if(str[i] == c)
			return i;
	}
	return -1;
}
//--------------------------------------------------------------
int last(const std::string& str, char c)
{
	for(int i=(int)str.size()-1; i>=0; --i)
	{
		if(str[i] == c)
			return i;
	}
	return -1;
}
//--------------------------------------------------------------
int nextValue(const std::string& str)
{
	int counter = 0;
	bool inAString = false;
	for(int i=0; i<(int)str.size(); ++i)
	{
		if(str[i] == '\"')
			inAString = !inAString;
		else if(!inAString)
		{
			if(str[i] == '[' || str[i] == '{')
				counter++;
			else if(str[i] == ']' || str[i] == '}')
				counter--;
			else if(counter == 0 && str[i]==',')
				return i;
		}
	}
	return -1;
}
//--------------------------------------------------------------
std::string getContained(const std::string& str, char c = '\"')
{
	// 0 1 2 3 4 5 6 7 8 9
	// " 1 2 3 4 5 6 7 8 "

	std::string result;

	int p1 = first(str, c);
	int p2 = getEndOf(str, p1); // last(str, c);

	result = substringPP(str, p1+1, p2-1);

	return result;
}
//--------------------------------------------------------------
std::string removeBracket(const std::string& str)
{
	// 0 1 2 3 4 5 6 7 8 9
	// " 1 2 3 4 5 6 7 8 "

	std::string result;

	int p1 = first(str, '{');
	int p2 = getEndOf(str, p1); // last(str, '}');

	result = substring(str, p1+1, p2-p1-1);

	return result;
}
//--------------------------------------------------------------
int getEndOf(const std::string& str, int beginPos)
{
	if(str[beginPos] == '[' || str[beginPos] == '{')
	{
		int pos = beginPos;
		int counter = 1;
		bool inAString = false;
		while(counter > 0)
		{
			pos++;
			if(str[pos] == '\"')
				inAString = !inAString;
			else if(!inAString)
			{
				if(str[pos] == '[' || str[pos] == '{')
					counter++;
				else if(str[pos] == ']' || str[pos] == '}')
					counter--;
			}
		}

		if( (str[beginPos]=='{' && str[pos]=='}') || (str[beginPos]=='[' && str[pos]==']') )
			return pos;
	}
	else if(str[beginPos] == '\"')
	{
		int pos = beginPos;
		pos = beginPos+1;
		while(pos < (int)str.size() && str[pos] != '\"')
			pos++;

		if(str[pos] == '\"')
			return pos;
	}
	return -1;
}
//--------------------------------------------------------------
bool isNumeric(const std::string& str)
{
    bool pos = str.size()>0 && str[0] >= '0' && str[0] <= '9';
    bool neg = str.size()>1 && str[0] == '-' && str[1] >='0' && str[1] <='9';

    return pos || neg;
}
//--------------------------------------------------------------
bool isString(const std::string& str)
{
	return str.size()>0 && str[0] == '"';
}
//--------------------------------------------------------------
bool isObject(const std::string& str)
{
	return str.size()>0 && str[0] == '{';	
}
//--------------------------------------------------------------
bool isArray(const std::string& str)
{
	return str.size()>0 && str[0] == '[';	
}
//--------------------------------------------------------------
bool isBoolean(const std::string& str)
{
	return str.size()>0 && (str[0] == 't' || str[0] == 'T' || str[0] == 'f' || str[0] == 'F');	
}

//--------------------------------------------------------------
std::pair<std::string,JsonValue> parsePair(const std::string& str);
JsonValue parseObject(const std::string& str);
JsonValue parseArray(const std::string& str);

//--------------------------------------------------------------
JsonValue parse(const std::string& str)
{
	JsonValue v;

	if(isBoolean(str))
    {
        v.type = JsonValue::Type::Boolean;
		v.boolean = asBoolean(str);
    }
	else if(isNumeric(str))
    {
        v.type = JsonValue::Type::Numeric;
		v.numeric = asDouble(str);
    }
	else if( isString(str) )
    {
        v.type = JsonValue::Type::String;
		v.string = getContained(str, '\"');
    }
	else if( isArray(str) )
    {
		v = parseArray(str);
        v.type = JsonValue::Type::Array;
    }
	else if( isObject(str) )
    {
		v = parseObject(str);
        v.type = JsonValue::Type::Object;
    }

	return v;
}
//-------------------------------------------------------
std::pair<std::string,JsonValue> parsePair(const std::string& str)
{
	int p1 = first(str, ':')-1;
	int p2 = p1+2;
	std::string before = substring(str, 0, p1);
	std::string after = substring(str, p2, (int)str.size()-p2);

    std::string key = getContained(before);
	JsonValue value = parse(after);

	return std::make_pair(key, value);
}
//--------------------------------------------------------------
JsonValue parseObject(const std::string& str)
{
	std::string contained = removeBracket(str);

	JsonValue v;

	std::string next = contained;
	while(next.size() > 0)
	{
		int pnext = nextValue(next); // first(next, ',');
		if(pnext == -1)
		{
            auto pair = parsePair(substring(next, 0, (int)next.size()));
            v.array.add(pair.first, pair.second);
			next = "";
		}
		else
		{
            auto pair = parsePair(substring(next, 0, pnext));
            v.array.add(pair.first, pair.second);
			next = substring(next, pnext+1, (int)next.size()-pnext);	
		}
	}

	return v;
}
//--------------------------------------------------------------
JsonValue parseArray(const std::string& str)
{
	JsonValue arr;

	int p1 = first(str, '[');
	int p2 = getEndOf(str, p1); // last(str, ']');
	std::string contained = substringPP(str, p1+1, p2-1 );
	std::string next = contained;
	while(next.size() > 0)
	{
		int pnext = nextValue(next); // first(next, ',');
		if(pnext == -1)
		{
            arr.array.values.push_back( parse(substring(next, 0, (int)next.size())) );
			next = "";
		}
		else
		{
            arr.array.values.push_back( parse(substring(next, 0, pnext)) );
			next = substring(next, pnext+1, (int)next.size()-pnext);	
		}
	}
	return arr;
}
//--------------------------------------------------------------
std::string removeSpace(const std::string& str)
{
	std::string result;
	bool inString = false;
	for(int i=0; i<(int)str.size(); ++i)
	{
		bool add = true;
		if(str[i] == '\"')
		{
			inString = !inString;
		}
		else if(!inString)
		{
			if(str[i] == '\n'
				|| str[i] == '\r'
				|| str[i] == ' '
				|| str[i] == '\t')
				add = false;
		}

		if(add)
			result.push_back(str[i]);
	}

	return result;
}
//--------------------------------------------------------------
bool check(const std::string& string)
{
	std::string str = removeSpace(string);

	std::vector<char> symbolPile;
	bool inString = false;

	for(int i=0; i<(int)str.size(); ++i)
	{
		if(str[i] == '\"')
		{
			inString = !inString;
		}
		else if(!inString)
		{
			if(str[i] == '[')
			{
				symbolPile.push_back('[');
			}
			else if(str[i] == ']')
			{
				if(symbolPile.back() != '[')
					return false;

				symbolPile.pop_back();
			}
			else if(str[i] == '{')
			{
				symbolPile.push_back('{');
			}
			else if(str[i] == '}')
			{
				if(symbolPile.back() != '{')
					return false;

				symbolPile.pop_back();
			}
		}
	}
	return true;
}
//--------------------------------------------------------------
static int s_jsonrw_tabIndex = 0;
//--------------------------------------------------------------
void serializeImpl(std::stringstream& ss, const JsonValue& json)
{
    bool compressed = false;
    
    if(json.type == JsonValue::Type::Boolean)
    {
        ss << (json.boolean ? "true" : "false");
    }
    else if(json.type == JsonValue::Type::Numeric)
    {
        ss << json.numeric;
    }
    else if(json.type == JsonValue::Type::String)
    {
        ss << "\"" << json.string << "\"";
    }
    else if(json.type == JsonValue::Type::Array)
    {
        ss  << "[";
        bool first = true;
        for(auto& item : json.array.values)
        {
			if (item.type == JsonValue::Type::Undefined) continue;
            if(!first) ss << ",";
            first = false;
            serializeImpl(ss, item);
        }
        ss  << "]";
    }
    else if(json.type == JsonValue::Type::Object)
    {
        ss  << "{";
        if(!compressed)
        {
            ss << "\n";
            s_jsonrw_tabIndex++;
            for(int i=0; i<s_jsonrw_tabIndex; ++i) ss << "    ";
        }
        bool first = true;
        for(auto& item : json.array.keys_indexes)
        {
			if (json.array.values[item.second].type == JsonValue::Type::Undefined) continue;
            if(!first)
            {
                ss << ",";
                if(!compressed)
                {
                    ss << "\n";
                    for(int i=0; i<s_jsonrw_tabIndex; ++i) ss << "    ";
                }
            }
            first = false;
            ss << "\"" << item.first << "\":";
            serializeImpl(ss, json.array.values[item.second]);
        }
        if(!compressed)
        {
            ss << "\n";
            s_jsonrw_tabIndex--;
            for(int i=0; i<s_jsonrw_tabIndex; ++i) ss << "    ";
        }
        ss  << "}";
    }
}
//--------------------------------------------------------------
std::string serialize(const JsonValue& json)
{
    s_jsonrw_tabIndex = 0;
    std::stringstream ss;
    serializeImpl(ss, json);
    return ss.str();
}
