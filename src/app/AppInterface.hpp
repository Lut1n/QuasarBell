#ifndef QUASAR_BELL_GUI_HPP
#define QUASAR_BELL_GUI_HPP

#include <list>
#include <vector>

#include "Audio/PcmData.hpp"
#include "App/UserFileInput.hpp"
#include "SignalOperation/OperationType.hpp"
#include "ImageOperation/ImageOperationType.hpp"
#include "Ui/UiNodeBoard.h"

enum NodeCategory
{
    NodeCategory_Signal,
    NodeCategory_Image,
    
    NodeCategory_None
};

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
    UserFileInput tgaInput;

    vec2 nodeToCreatePos;
    NodeCategory nodeToCreateCategory = NodeCategory_None;

    // qb::OperationType or qb::ImageOperationType depending on nodeToCreateCategory
    size_t nodeToCreateType = qb::OperationType_None;

private:
    std::vector<std::string> operationNames;
    std::vector<std::string> imageOperationNames;

    AboutPanel _aboutPanel;
};

#endif // QUASAR_BELL_GUI_HPP
