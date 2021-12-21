#ifndef GUI_BASE_OPERATION_H
#define GUI_BASE_OPERATION_H

#include "App/BaseAttributes.hpp"


//--------------------------------------------------------------
struct BaseOperationVisitor
{
    Tree<BaseAttributes*> data;
    Tree<BaseAttributes*>* current;
    virtual ~BaseOperationVisitor() = default;
};

//--------------------------------------------------------------
struct BaseOperation
{
    virtual ~BaseOperation() = default;

    void startSamplingGraph(int d=0);
    virtual void startSampling(int d=0);

    virtual bool apply(size_t index, BaseOperationVisitor& visitor) = 0;
};

#endif // GUI_BASE_OPERATION_H
