#ifndef QUASAR_BELL_GUI_HPP
#define QUASAR_BELL_GUI_HPP

#include <list>
#include <vector>

#include "Audio/PcmData.hpp"
#include "App/UserFileInput.hpp"
#include "SignalOperation/OperationType.hpp"
#include "Ui/UiNodeBoard.h"

struct KeyEvent
{
    char key;
    bool pressed;
    double time;
};

class AppInterface
{
public:
    AppInterface();
    ~AppInterface() = default;
    
    void display();

    static AppInterface& get();

public:
    bool resetProject = false;
    UiNodeBoard* nodeboard = nullptr;
    UserFileInput fileInput;
    UserFileInput waveInput;

    vec2 nodeToCreatePos;
    qb::OperationType nodeToCreateType = qb::OperationType_None;

private:
    std::vector<std::string> operationNames;

    AboutPanel _aboutPanel;
};

#endif // QUASAR_BELL_GUI_HPP
