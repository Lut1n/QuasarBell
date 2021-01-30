#include "io/FileIO.hpp"

#include <fstream>
#include <filesystem>

#include "io/json/jsonrw.hpp"


//--------------------------------------------------------------
std::string loadTextFile(const std::string& filename)
{
    std::ifstream ifs(filename);
    std::string buffer( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>()    ) );
    return buffer;
}

//--------------------------------------------------------------
void saveTextFile(const std::string& filename, const std::string& text)
{
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    std::ofstream ofs(filename);
    ofs << text;
}

//--------------------------------------------------------------
JsonValue loadJsonFile(const std::string& filename)
{
    std::string text = loadTextFile(filename);
    text = removeSpace(text);
    return parse(text);
}

//--------------------------------------------------------------
void saveJsonFile(const std::string& filename, JsonValue& json)
{
    std::string text = serialize(json);
    saveTextFile(filename, text);
}