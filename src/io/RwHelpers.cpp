#include "io/RwHelpers.hpp"

//--------------------------------------------------------------
std::string toString(WaveEdit::Wave e)
{
    static const char* items[] = {"Sin", "Saw", "Square", "Noise_1", "Noise_2", "Noise_3", "Noise_4"};
    return items[(unsigned)e];
}
//--------------------------------------------------------------
void stringTo(const std::string& str, WaveEdit::Wave& e)
{
    static const char* items[] = {"Sin", "Saw", "Square", "Noise_1", "Noise_2", "Noise_3", "Noise_4"};
    for(int i=0; i<7; ++i)
        if(str == items[i]) e = (WaveEdit::Wave)i;
}
