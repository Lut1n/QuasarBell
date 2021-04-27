#include "signal/operations/Operations.hpp"

#include <iostream>

#include "io/json/Json.hpp"

//--------------------------------------------------------------
FloatInput::FloatInput()
{
    initialize({},{DataType_Float});
    makeProperty({"value", DataType_Float, &value});
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

//--------------------------------------------------------------
CubicSampler::CubicSampler()
{
    initialize({DataType_Float,DataType_Float,DataType_Float,DataType_Float,DataType_Float},{DataType_Float});
    makeProperty({"value", DataType_Float, &value});
    makeProperty({"speed", DataType_Float, &speed});
    makeProperty({"acc", DataType_Float, &acc});
    makeProperty({"jerk", DataType_Float, &jerk});
    makeProperty({"reset", DataType_Float, &reset});
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

//--------------------------------------------------------------
PolynomialSampler::PolynomialSampler()
{
    initialize({},{DataType_Float});
    makeProperty({"count", DataType_Int, &count});
    makeProperty({"reset", DataType_Float, &reset});
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
    count = jArray.count();
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
    initialize({DataType_Float, DataType_Float},{DataType_Float});
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
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

//--------------------------------------------------------------
SubOperation::SubOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float});
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
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

//--------------------------------------------------------------
MultOperation::MultOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float});
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
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

//--------------------------------------------------------------
DivOperation::DivOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float});
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
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

//--------------------------------------------------------------
ClampOperation::ClampOperation()
{
    initialize({DataType_Float, DataType_Float, DataType_Float},{DataType_Float});
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"minVal", DataType_Float, &minVal});
    makeProperty({"maxVal", DataType_Float, &maxVal});
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

//--------------------------------------------------------------
AbsOperation::AbsOperation()
{
    initialize({DataType_Float},{DataType_Float});
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
    initialize({DataType_Float},{DataType_Float});
    makeProperty({"delay", DataType_Float, &delay});
    makeProperty({"scale", DataType_Float, &scale});
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

//--------------------------------------------------------------
OutputOperation::OutputOperation()
{
    initialize({DataType_Float},{});
    makeProperty({"range", DataType_Float, &range});
    makeProperty({"duration", DataType_Float, &duration});
    makeProperty({"sample-rate", DataType_Int, &sampleRate});
    makeProperty({"sample-bits", DataType_Int, &sampleBits});
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