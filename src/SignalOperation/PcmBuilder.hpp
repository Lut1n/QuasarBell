#ifndef QUASAR_BELL_PCM_BUILDER_HPP
#define QUASAR_BELL_PCM_BUILDER_HPP

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "Core/Vec2.h"
#include "Core/Math.hpp"
#include "ImageOperation/ImageOperationType.hpp"
#include "App/BaseOperationNode.hpp"

//--------------------------------------------------------------
namespace qb
{
    struct OperationData
    {
        BaseOperationDataType type = BaseOperationDataType::Undefined;
        // size_t type = 0;
        size_t count = 0;
        union
        {
            int ivec[4];
            float fvec[4];
            bool bvec[4];
        };
    };
    
    struct Time
    {
        float t = 0.0f;
        float sec = 0.0f;
        float elapsed = 0.0f;
        float duration = 1.0f;
        // mutable SignalOperation* dstOp = nullptr;
    };

    struct PcmBuilderVisitor : public BaseOperationVisitor
    {
        Time time;
        float phase = 0.0f;
        OperationData data;
    };
};


#endif // QUASAR_BELL_PCM_BUILDER_HPP