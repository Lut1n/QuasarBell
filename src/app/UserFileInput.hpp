#ifndef QUASAR_BELL_GUI_FILE_INPUT_HPP
#define QUASAR_BELL_GUI_FILE_INPUT_HPP

#include <iostream>

#include "ReaderWriter/FileExplorer.hpp"

//--------------------------------------------------------------
class UserFileInput
{
public:
    enum Req
    {
        Nothing,
        Save_Prj,
        Load_Prj,
        Export_Wav,
        Export_Tga
    };
    
    UserFileInput(const std::string& title, const std::string& ext, const std::string& defaultPath);
    virtual ~UserFileInput();
    
    void open();
    void close();
    
    void display();
    void displayItem(const char* text, UserFileInput::Req req);

public:
    Req request = Nothing;
    bool requestOpen = false;
    bool confirmed = false;
    
    std::string filepath;
    
private:
    FileExplorer _explorer;
    std::string _title;
    char _editBuffer[512] = "";
    std::string _ext;
};

//--------------------------------------------------------------
class AboutPanel
{
public:
    AboutPanel();
    
    void open();
    void close();
    void display();
    
private:
    bool requestOpen = false;
};

#endif // QUASAR_BELL_GUI_FILE_INPUT_HPP
