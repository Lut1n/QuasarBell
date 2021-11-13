#include "ImageOperation/ImageBasicOperators.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
ImageBasicOperators::ImageBasicOperators(qb::ImageOperationType type, const std::string& operatorSymbol)
    : ImageOperation(type)
    , _operatorSymbol(operatorSymbol)
{
    makeInput("in1", BaseOperationDataType::Float, UiPin::Type_S2d_3d);
    makeInput("in2", BaseOperationDataType::Float, UiPin::Type_S2d_3d);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("in1", &_in1, 0.0f, 10.0f);
    makeProperty("in2", &_in2, 0.0f, 10.0f);
}
//--------------------------------------------------------------
bool ImageBasicOperators::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    bool samples3d0 = inputHasFlag(0, UiPin::Type_S3d);
    bool samples3d1 = inputHasFlag(1, UiPin::Type_S3d);
    
    std::string operand1, operand2, uvArg;
    if ( samples3d0 )
        operand1 = sampleSdfInput(0, visitor, {_in1,_in1,_in1,1.0f});
    else
        operand1 = pushOpOrInput(0,visitor, {_in1,_in1,_in1,1.0f});
    if ( samples3d1 )
        operand2 = sampleSdfInput(1, visitor, {_in2,_in2,_in2,1.0f});
    else
        operand2 = pushOpOrInput(1,visitor, {_in2,_in2,_in2,1.0f});

    size_t opId = context.getNextVa();
    std::string glsl = "vec4 $1 = vec4($2.xyz $3 $4.xyz, 1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), operand1, _operatorSymbol, operand2});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageBuiltInFunc::ImageBuiltInFunc(qb::ImageOperationType type, const std::string& funcName, const std::vector<std::string>& argNames)
    : ImageOperation(type)
    , argNames(argNames)
{
    argValues.resize(argNames.size(), 0.0);
    idsCache.resize(argNames.size() + 1);
    glslTemplate = "vec4 $1 = vec4(" + funcName + "(";
    for(size_t i=0; i<argNames.size(); ++i)
    {
        makeProperty(argNames[i], BaseOperationDataType::Float, &argValues[i]);
        makeInput(argNames[i], BaseOperationDataType::Float);
        if(i>0) glslTemplate += ",";
        glslTemplate += "$" + std::to_string(i+2) + ".xyz";
    }
    glslTemplate += "),1.0);\n";
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool ImageBuiltInFunc::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    for(size_t i=0; i<argValues.size(); ++i)
    {
        float arg = argValues[i];
        idsCache[i+1] = pushOpOrInput(i,visitor,{arg,arg,arg,arg});
    }

    size_t opId = context.getNextVa();
    idsCache[0] = qb::va(opId);

    std::string glsl = qb::replaceArgs(glslTemplate, idsCache);
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}

//--------------------------------------------------------------
MAKE_BUILTIN_OPERATION_FUNC(Mix, "mix", QB_ARGS({"in1", "in2", "delta"}))
MAKE_BUILTIN_OPERATION_FUNC(Clamp, "clamp", QB_ARGS({"in1", "edge0", "edge1"}))
MAKE_BUILTIN_OPERATION_FUNC(Dot, "dot", QB_ARGS({"in1", "in2"}))
MAKE_BUILTIN_OPERATION_FUNC(Cross, "cross", QB_ARGS({"in1", "in2"}))
MAKE_BUILTIN_OPERATION_FUNC(Step, "step", QB_ARGS({"edge", "in"}))
MAKE_BUILTIN_OPERATION_FUNC(Sin, "sin", QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_FUNC(Cos, "cos", QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_FUNC(Tan, "tan", QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_FUNC(Mod, "mod", QB_ARGS({"in", "mod"}))
MAKE_BUILTIN_OPERATION_FUNC(Pow, "pow", QB_ARGS({"in", "pow"}))
MAKE_BUILTIN_OPERATION_FUNC(Sqrt, "sqrt", QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_FUNC(Abs, "abs", QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_FUNC(Log, "log", QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_FUNC(Exp, "exp", QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_FUNC(Min, "min", QB_ARGS({"in1","in2"}))
MAKE_BUILTIN_OPERATION_FUNC(Max, "max", QB_ARGS({"in1","in2"}))

//--------------------------------------------------------------
MAKE_BASIC_OPERATOR_SYMBOL(Add, "+")
MAKE_BASIC_OPERATOR_SYMBOL(Sub, "-")
MAKE_BASIC_OPERATOR_SYMBOL(Mult, "*")
MAKE_BASIC_OPERATOR_SYMBOL(Div, "/")