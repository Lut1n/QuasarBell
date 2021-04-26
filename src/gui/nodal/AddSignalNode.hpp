#ifndef GUI_ADD_SIGNAL_NODE_H
#define GUI_ADD_SIGNAL_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"

struct AddSignalNode : public SignalOperationNode
{
    AddSignalNode();
    void displayProperties() override;
    
public:
    AddOperation add;
};

struct SubSignalNode : public SignalOperationNode
{
    SubSignalNode();
    void displayProperties() override;
    
public:
    SubOperation sub;
};

struct MultSignalNode : public SignalOperationNode
{
    MultSignalNode();
    void displayProperties() override;
    
public:
    MultOperation mult;
};

struct DivSignalNode : public SignalOperationNode
{
    DivSignalNode();
    void displayProperties() override;
    
public:
    DivOperation div;
};

struct ClampSignalNode : public SignalOperationNode
{
    ClampSignalNode();
    void displayProperties() override;
    
public:
    ClampOperation clamp;
};

struct AbsSignalNode : public SignalOperationNode
{
    AbsSignalNode();
    void displayProperties() override;
    
public:
    AbsOperation abs;
};

struct TimeScaleNode : public SignalOperationNode
{
    TimeScaleNode();
    void displayProperties() override;
    
public:
    TimeScale timeScale;
};


#endif // GUI_ADD_SIGNAL_NODE_H
