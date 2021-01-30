#ifndef QUASAR_BELL_GUI_FILE_INPUT_HPP
#define QUASAR_BELL_GUI_FILE_INPUT_HPP

#include <iostream>

//--------------------------------------------------------------
class UserFileInput
{
public:
    enum Req
    {
        Nothing,
        Save_Prj,
        Load_Prj,
        Export_Wav
    };
    
    UserFileInput(const std::string& title);
    virtual ~UserFileInput();
    
    void open();
    void close();
    
    void display();

public:
    Req request = Nothing;
    bool requestOpen = false;
    bool confirmed = false;
    
    std::string filepath;
    
private:
    std::string _title;
    char _editBuffer[512] = "";
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
