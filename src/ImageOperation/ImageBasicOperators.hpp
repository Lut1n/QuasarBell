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

//--------------------------------------------------------------
struct ImageAdd : public ImageBasicOperators { ImageAdd(); };
struct ImageSub : public ImageBasicOperators { ImageSub(); };
struct ImageMult : public ImageBasicOperators{ ImageMult(); };
struct ImageDiv : public ImageBasicOperators{ ImageDiv(); };


#endif // QUASAR_BELL_IMAGE_BASIC_OPERATORS_HPP