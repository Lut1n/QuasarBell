#include "Core/PseudoRand.hpp"

//--------------------------------------------------------------
namespace qb
{
    static int s_hold_rand = 0;
        
    void platform_ind_srand(int seed)
    {
        s_hold_rand = seed;
    }
    
    int platform_ind_rand()
    {
        return(((s_hold_rand = s_hold_rand * 214013L + 2531011L) >> 16) & 0x7fff);
    }
}

//--------------------------------------------------------------
void qb::noiseSeed(int seed)
{
    platform_ind_srand(seed);
}

//--------------------------------------------------------------
float qb::noiseValue()
{
    const int R_MAX = 100;
    return (float)(platform_ind_rand()%(1+R_MAX)) / R_MAX;
}