#ifndef QUASAR_BELL_SIGNAL_OPERATION_HPP
#define QUASAR_BELL_SIGNAL_OPERATION_HPP

#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>

#include "SignalOperation/PcmBuilder.hpp"
#include "App/BaseOperationNode.hpp"

//--------------------------------------------------------------
struct SignalOperation;

struct SignalPreview
{
    std::array<float, 32> data;
    float maxVal = 1.0;
    bool hasChange = true;

    void dirty();
    void compute(SignalOperation* operation);
};

//--------------------------------------------------------------
struct SignalOperation : public BaseOperation
{
    SignalOperation();
    virtual ~SignalOperation() = default;
    
    void update();
    bool sampleInput(size_t index, qb::PcmBuilderVisitor& visitor);

    float inputOrProperty(size_t index, qb::PcmBuilderVisitor& visitor, float property);

    virtual std::string name() const;
    bool sample(size_t index, BaseOperationVisitor& visitor) override;
    virtual bool sample(size_t index, qb::PcmBuilderVisitor& visitor);
    
    void uiPreview() override;
    void onInputConnectionChanged() override;
    void onPropertiesChanged() override;
    void dirty();

    SignalPreview preview;

private:
    void updateAllChildren();
};

#endif // QUASAR_BELL_SIGNAL_OPERATION_HPP