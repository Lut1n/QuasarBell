#include "signal/SignalOperation.hpp"

/*OperationData::~OperationData()
{
    _desalloc();
}

void OperationData::alloc(OperationDataType t, size_t c)
{
    if (type != t || count != c)
    {
        _desalloc();
        type = t;
        count = c;)
        if (type == DataType_Bool) bvec = new bool[count];
        if (type == DataType_Int) ivec = new int[count];
        if (type == DataType_Float) fvec = new float[count];
    }
}

void OperationData::_desalloc()
{
    if (type == DataType_Bool) delete [] bvec;
    if (type == DataType_Int) delete [] ivec;
    if (type == DataType_Float) delete [] fvec;

    type = DataType_Undefined;
    count = 0;
}*/


//--------------------------------------------------------------
OperationDataType SignalOperation::getInputType(size_t index) const
{
    if (index < inputs.size()) return inputs[index].type;
    return DataType_Undefined;
}

//--------------------------------------------------------------
OperationDataType SignalOperation::getOuputType(size_t index) const
{
    if (index < outputs.size()) return outputs[index].type;
    return DataType_Undefined;
}

//--------------------------------------------------------------
void SignalOperation::setInput(size_t index, SignalOperation* src, size_t srcIndex)
{
    setConnection(src, srcIndex, this, index);
}

//--------------------------------------------------------------
void SignalOperation::setOuput(size_t index, SignalOperation* dst, size_t dstIndex)
{
    setConnection(this, index, dst, dstIndex);
}

//--------------------------------------------------------------
SignalOperationConnection* SignalOperation::getInput(size_t index)
{
    if (index < inputs.size())
        return &inputs[index];
    return nullptr;
}

//--------------------------------------------------------------
SignalOperationConnection* SignalOperation::getOutput(size_t index)
{
    if (index < outputs.size())
        return &outputs[index];
    return nullptr;
}

//--------------------------------------------------------------
OperationData SignalOperation::getInputData(size_t index)
{
    SignalOperationConnection* co = getInput(index);
    if (co->operation)
        return co->operation->getOutputData(inputs[index].index);
    else
        return OperationData{DataType_Undefined};
}

//--------------------------------------------------------------
void SignalOperation::initialize(const std::vector<OperationDataType>& input, const std::vector<OperationDataType>& output)
{
    inputs.resize(input.size());
    for (int i=0; i<inputs.size(); ++i)
    {
        inputs[i].type = input[i];
        inputs[i].index = 0;
        inputs[i].operation = nullptr;
    }
    outputs.resize(output.size());
    for (int i=0; i<outputs.size(); ++i)
    {
        outputs[i].type = output[i];
        outputs[i].index = 0;
        outputs[i].operation = nullptr;
    }
}

//--------------------------------------------------------------
void SignalOperation::update()
{
    updateData();
    updateAllChildren();
}

//--------------------------------------------------------------
OperationData SignalOperation::getOutputData(size_t index) const
{
    return OperationData(); //getData(index);
}

//--------------------------------------------------------------
void SignalOperation::updateAllChildren()
{
    for (int i=0; i<outputs.size(); ++i)
    {
        if(outputs[i].operation)
            outputs[i].operation->update();
    }
}

//--------------------------------------------------------------
void SignalOperation::setConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx)
{
    remConnection(src, srcIdx, nullptr, 0);
    remConnection(nullptr, 0, dst, dstIdx);
    
    if (src && dst && srcIdx < src->outputs.size() && dstIdx < dst->inputs.size() && dst->inputs[dstIdx].type == src->outputs[srcIdx].type)
    {
        src->outputs[srcIdx].operation = dst;
        src->outputs[srcIdx].index = dstIdx;
        
        dst->inputs[dstIdx].operation = src;
        dst->inputs[dstIdx].index = srcIdx;
    }
}

//--------------------------------------------------------------
void SignalOperation::remConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx)
{
    if(src && srcIdx < src->outputs.size())
    {
        auto op = src->outputs[srcIdx].operation;
        auto index = src->outputs[srcIdx].index;
        if(op)
        {
            op->inputs[index].operation = nullptr;
            op->inputs[index].index = 0;
        }
        src->outputs[srcIdx].operation = nullptr;
        src->outputs[srcIdx].index = 0;
    }
    if (dst && dstIdx < dst->inputs.size())
    {
        auto op = dst->inputs[dstIdx].operation;
        auto index = dst->inputs[dstIdx].index;
        if(op)
        {
            op->outputs[index].operation = nullptr;
            op->outputs[index].index = 0;
        }
        dst->inputs[dstIdx].operation = nullptr;
        dst->inputs[dstIdx].index = 0;
    }
}
//--------------------------------------------------------------
void SignalOperation::updateData()
{
    // to implement by children
}

//--------------------------------------------------------------
OperationData SignalOperation::getData(size_t index, float t)
{
    // to implement by children
}