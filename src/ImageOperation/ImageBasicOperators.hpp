#ifndef QUASAR_BELL_IMAGE_BASIC_OPERATORS_HPP
#define QUASAR_BELL_IMAGE_BASIC_OPERATORS_HPP

#include "ImageOperation/ImageOperation.hpp"

//--------------------------------------------------------------
struct ImageBasicOperators : public ImageOperation
{
    ImageBasicOperators(qb::ImageOperationType type, const std::string& operatorSymbol);
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;
    std::string _operatorSymbol;
    float _in1 = 0.0f;
    float _in2 = 0.0f;
};

#define MAKE_BASIC_OPERATOR(funcName) \
    struct Image##funcName : public ImageBasicOperators {Image##funcName();};

#define MAKE_BASIC_OPERATOR_SYMBOL(funcName, opSymbol)\
    Image##funcName::Image##funcName() : ImageBasicOperators(qb::ImageOperationType_##funcName, opSymbol){}

//--------------------------------------------------------------
struct ImageBuiltInFunc : public ImageOperation
{
    ImageBuiltInFunc(qb::ImageOperationType type, const std::string& funcName, const std::vector<std::string>& argNames);
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    std::string glslTemplate;
    std::vector<std::string> argNames;
    std::vector<float> argValues;
    std::vector<std::string> idsCache;
};

//--------------------------------------------------------------
#define QB_ARGS(...) __VA_ARGS__

#define MAKE_BUILTIN_OPERATION(funcName) \
    struct Image##funcName : public ImageBuiltInFunc {Image##funcName();};

#define MAKE_BUILTIN_OPERATION_FUNC(funcName, func, args)\
    Image##funcName::Image##funcName() : ImageBuiltInFunc(qb::ImageOperationType_##funcName, func, args){}

//--------------------------------------------------------------
MAKE_BUILTIN_OPERATION(Mix)
MAKE_BUILTIN_OPERATION(Clamp)
MAKE_BUILTIN_OPERATION(Dot)
MAKE_BUILTIN_OPERATION(Cross)
MAKE_BUILTIN_OPERATION(Step)
MAKE_BUILTIN_OPERATION(Sin)
MAKE_BUILTIN_OPERATION(Cos)
MAKE_BUILTIN_OPERATION(Tan)
MAKE_BUILTIN_OPERATION(Mod)
MAKE_BUILTIN_OPERATION(Pow)
MAKE_BUILTIN_OPERATION(Sqrt)
MAKE_BUILTIN_OPERATION(Abs)
MAKE_BUILTIN_OPERATION(Log)
MAKE_BUILTIN_OPERATION(Exp)
MAKE_BUILTIN_OPERATION(Min)
MAKE_BUILTIN_OPERATION(Max)

//--------------------------------------------------------------
MAKE_BASIC_OPERATOR(Add)
MAKE_BASIC_OPERATOR(Sub)
MAKE_BASIC_OPERATOR(Mult)
MAKE_BASIC_OPERATOR(Div)

#endif // QUASAR_BELL_IMAGE_BASIC_OPERATORS_HPP