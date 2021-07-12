#ifndef QUASAR_BELL_RM_OPERATION_TYPE_HPP
#define QUASAR_BELL_RM_OPERATION_TYPE_HPP

#include <vector>
#include <string>

//--------------------------------------------------------------
namespace qb
{
    enum SdfOperationType
    {
        SdfOperationType_Sphere = 0,
        SdfOperationType_Box,
        SdfOperationType_Union,
        SdfOperationType_SmoothUnion,
        SdfOperationType_Transform,
        SdfOperationType_HighResOutput,

        SdfOperationType_Count,

        SdfOperationType_None
    };

    std::string getSdfOperationName(SdfOperationType type);
    SdfOperationType getSdfOperationType(const std::string& name);
    std::vector<std::string> getSdfOperationNames();
};


#endif // QUASAR_BELL_RM_OPERATION_TYPE_HPP