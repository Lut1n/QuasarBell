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
    makeInput("in1", ImageDataType_Float);
    makeInput("in2", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("in1", &_in1, 0.0f, 10.0f);
    makeProperty("in2", &_in2, 0.0f, 10.0f);
}
//--------------------------------------------------------------
bool ImageBasicOperators::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    std::string operand1 = pushOpOrInput(0,t,visitor, {_in1,_in1,_in1,1.0f});
    std::string operand2 = pushOpOrInput(1,t,visitor, {_in2,_in2,_in2,1.0f});

    size_t opId = context.getNextVa();
    std::string glsl = "vec4 $1 = vec4($2.xyz $3 $4.xyz, 1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), operand1, _operatorSymbol, operand2});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}

//--------------------------------------------------------------
ImageAdd::ImageAdd() : ImageBasicOperators(qb::ImageOperationType_Add, "+") {}
ImageSub::ImageSub() : ImageBasicOperators(qb::ImageOperationType_Sub, "-") {}
ImageMult::ImageMult() : ImageBasicOperators(qb::ImageOperationType_Mult, "*") {}
ImageDiv::ImageDiv() : ImageBasicOperators(qb::ImageOperationType_Div, "/") {}