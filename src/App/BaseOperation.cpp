#include "App/BaseOperation.hpp"


//--------------------------------------------------------------
void BaseOperation::startSamplingGraph(int /*d*/)
{ 
    /*for (auto input : inputs)
    {
        for(auto& ref : input.refs)
            if (ref.operation) ref.operation->startSamplingGraph(d+1);
    }
    startSampling(d);*/
}
//--------------------------------------------------------------
void BaseOperation::startSampling(int /*d*/)
{
    // to implement by children
}