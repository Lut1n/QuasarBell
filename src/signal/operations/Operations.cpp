#include "signal/operations/Operations.hpp"

#include <iostream>

#include "io/json/Json.hpp"

//--------------------------------------------------------------
FloatInput::FloatInput()
{
    initialize({},{DataType_Float}, {
            {"value",DataType_Float}});
}
//--------------------------------------------------------------
void FloatInput::validate()
{
}
//--------------------------------------------------------------
OperationData FloatInput::sample(size_t index, const Time& t)
{
    OperationData data;
    data.type = DataType_Float;
    data.count = 1;
    data.fvec[0] = value;
    return data;
}

void FloatInput::getProperty(size_t i, float& value) const
{
    value = this->value;
}
void FloatInput::setProperty(size_t i, float value)
{
    this->value = value;
}

//--------------------------------------------------------------
CubicSampler::CubicSampler()
{
    initialize({DataType_Float,DataType_Float,DataType_Float,DataType_Float,DataType_Float},{DataType_Float}, {
            {"value",DataType_Float},
            {"speed",DataType_Float},
            {"acc",DataType_Float},
            {"jerk",DataType_Float},
            {"reset",DataType_Float}});
}
//--------------------------------------------------------------
void CubicSampler::validate()
{
}
//--------------------------------------------------------------
OperationData CubicSampler::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);
    OperationData d = sampleInput(3, t);
    OperationData e = sampleInput(4, t);

    float va = a.type != DataType_Error ? a.fvec[0] : value;
    float sp = b.type != DataType_Error ? b.fvec[0] : speed;
    float ac = c.type != DataType_Error ? c.fvec[0] : acc;
    float je = d.type != DataType_Error ? d.fvec[0] : jerk;
    float rs = e.type != DataType_Error ? e.fvec[0] : reset;

    auto sampler = [va,sp,ac,je,rs](float t)
    {
        if(rs != 0.0f)
        {
            while(t>=rs) t -= rs;
        }
        return va + (sp + (ac + je * t) * t) * t;
    };

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = sampler(t.t);
    return data;
}

void CubicSampler::getProperty(size_t i, float& value) const
{
    if (i == 0)
        value = this->value;
    else if (i == 1)
        value = this->speed;
    else if (i == 2)
        value = this->acc;
    else if (i == 3)
        value = this->jerk;
    else if (i == 4)
        value = this->reset;
}
void CubicSampler::setProperty(size_t i, float value)
{
    if (i == 0)
        this->value = value;
    else if (i == 1)
        this->speed = value;
    else if (i == 2)
        this->acc = value;
    else if (i == 3)
        this->jerk = value;
    else if (i == 4)
        this->reset = value;
}

//--------------------------------------------------------------
PolynomialSampler::PolynomialSampler()
{
    initialize({},{DataType_Float}, {
            {"count",DataType_Float},
            {"reset",DataType_Float}});
    coefs.resize(1, 0);
}
//--------------------------------------------------------------
void PolynomialSampler::validate()
{
}
//--------------------------------------------------------------
OperationData PolynomialSampler::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);

    float x = t.t;
    if(reset > 0.0)
    {
        while(x>=reset) x -= reset;
    }
    float res = 0.0;
    float ve = 1.0;
    for(auto coef : coefs)
    {
        res += coef * ve;
        ve *= x;
    }

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = res;
    return data;
}
void PolynomialSampler::getProperty(size_t i, float& value) const
{
    if (i == 0)
        value = this->count;
    else if (i == 1)
        value = this->reset;
}
void PolynomialSampler::setProperty(size_t i, float value)
{
    if (i == 0)
        this->count = value;
    else if (i == 1)
        this->reset = value;
}
void PolynomialSampler::saveCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("coefs");
    int index = 0;
    for (auto coef : coefs)
    {
        jArray.setPath(index++).set(coef);
    }
}
void PolynomialSampler::loadCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("coefs");
    count = jArray.array.values.size();
    coefs.resize(count);
    int index = 0;
    for(auto& jcoef : jArray.array.values)
    {
        coefs[index++] = jcoef.getNumeric();
    }
}
//--------------------------------------------------------------
AddOperation::AddOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float}, {
            {"input1",DataType_Float},
            {"input2",DataType_Float}});
}
//--------------------------------------------------------------
void AddOperation::validate()
{
}
//--------------------------------------------------------------
OperationData AddOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 + i2;
    return data;
}

void AddOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = input2;
}
void AddOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        input2 = value;
}

//--------------------------------------------------------------
SubOperation::SubOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float}, {
            {"input1",DataType_Float},
            {"input2",DataType_Float}});
}
//--------------------------------------------------------------
void SubOperation::validate()
{
}
//--------------------------------------------------------------
OperationData SubOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 - i2;
    return data;
}

void SubOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = input2;
}
void SubOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        input2 = value;
}


//--------------------------------------------------------------
MultOperation::MultOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float}, {
            {"input1",DataType_Float},
            {"input2",DataType_Float}});
}
//--------------------------------------------------------------
void MultOperation::validate()
{
}
//--------------------------------------------------------------
OperationData MultOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 * i2;
    return data;
}

void MultOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = input2;
}
void MultOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        input2 = value;
}

//--------------------------------------------------------------
DivOperation::DivOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float}, {
            {"input1",DataType_Float},
            {"input2",DataType_Float}});
}
//--------------------------------------------------------------
void DivOperation::validate()
{
}
//--------------------------------------------------------------
OperationData DivOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i2!=0.0 ? i1/i2 : i1;
    return data;
}

void DivOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = input2;
}
void DivOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        input2 = value;
}

//--------------------------------------------------------------
ClampOperation::ClampOperation()
{
    initialize({DataType_Float, DataType_Float, DataType_Float},{DataType_Float}, {
            {"input1",DataType_Float},
            {"minVal",DataType_Float},
            {"maxVal",DataType_Float}});
}
//--------------------------------------------------------------
void ClampOperation::validate()
{
}
//--------------------------------------------------------------
OperationData ClampOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float e1 = b.type == DataType_Float ? b.fvec[0] : minVal;
    float e2 = c.type == DataType_Float ? c.fvec[0] : maxVal;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1>e2 ? e2 : (i1<e1 ? e1 : i1);
    return data;
}

void ClampOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = minVal;
    else if (i==2)
        value = maxVal;
}
void ClampOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        minVal = value;
    else if (i==2)
        maxVal = value;
}

//--------------------------------------------------------------
AbsOperation::AbsOperation()
{
    initialize({DataType_Float},{DataType_Float}, {});
}
//--------------------------------------------------------------
void AbsOperation::validate()
{
}
//--------------------------------------------------------------
OperationData AbsOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);

    float val = a.type == DataType_Float ? a.fvec[0] : 0.0;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = val>0.0 ? val : -val;
    return data;
}


//--------------------------------------------------------------
TimeScale::TimeScale()
{
    initialize({DataType_Float},{DataType_Float}, {
            {"delay",DataType_Float},
            {"scale",DataType_Float}});
}
//--------------------------------------------------------------
void TimeScale::validate()
{
}
//--------------------------------------------------------------
OperationData TimeScale::sample(size_t index, const Time& t)
{
    Time time2 = t;
    time2.duration = t.duration * scale;
    time2.elapsed = t.elapsed * scale;
    time2.t = t.t*scale - delay;
    time2.sec =  time2.t * time2.duration;

    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, time2);

    data.fvec[0] = a.type == DataType_Float ? a.fvec[0] : 0.0;
    data.type = output->type;
    data.count = output->count;
    return data;
}

void TimeScale::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = delay;
    if (i==1)
        value = scale;
}
void TimeScale::setProperty(size_t i, float value)
{
    if (i==0)
        delay = value;
    if (i==1)
        scale = value;
}

//--------------------------------------------------------------
OutputOperation::OutputOperation()
{
    initialize({DataType_Float},{}, {
            {"range",DataType_Float},
            {"duration",DataType_Float},
            {"sample-rate",DataType_Float},
            {"sample-bits",DataType_Float}});
}
//--------------------------------------------------------------
void OutputOperation::validate()
{
    // auto* input = getInput(0);
}
//--------------------------------------------------------------
OperationData OutputOperation::sample(size_t index, const Time& t)
{
    return sampleInput(index, t);
}

void OutputOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = range;
    if (i==1)
        value = duration;
    if (i==2)
        value = sampleRate;
    if (i==3)
        value = sampleBits;
}
void OutputOperation::setProperty(size_t i, float value)
{
    if (i==0)
        range = value;
    if (i==1)
        duration = value;
    if (i==2)
        sampleRate = value;
    if (i==3)
        sampleBits = value;
}