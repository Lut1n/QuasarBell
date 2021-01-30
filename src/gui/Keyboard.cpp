#include "Keyboard.hpp"

#include "imgui.h" 

//--------------------------------------------------------------
enum ImGuiMouseButton_
{
    ImGuiMouseButton_Left = 0,
    ImGuiMouseButton_Right = 1,
    ImGuiMouseButton_Middle = 2,
    ImGuiMouseButton_COUNT = 5
};

//--------------------------------------------------------------
Keyboard::Keyboard(unsigned firstId, unsigned lastId)
    : _keyFirstIndex(firstId)
    , _keyLastIndex(lastId)
{
    _keys.resize(_keyLastIndex - _keyFirstIndex + 1, false);
    size_t white_count = _keys.size() * 7 / 12;
    size_t black_count = _keys.size() * 5 / 12;
    _whiteKeys.resize(white_count);
    _blackKeys.resize(black_count);
}
//--------------------------------------------------------------
void Keyboard::reset()
{
    for (size_t i = 0; i<_keys.size(); ++i)
    {
        _keys[i] = false;
    }
}
//--------------------------------------------------------------
void Keyboard::setPressed(int i)
{
    _keys[i-_keyFirstIndex]=true;
}
//--------------------------------------------------------------
bool Keyboard::isOverKey(float x, float y, const KeyView& key)
{
    float yOffset = ImGui::GetCursorScreenPos().y;
    return x > key.oft && x < key.oft+_keyWidth && y > yOffset && y < yOffset + _keyHeight * (key.black?0.6:1.0);
}
//--------------------------------------------------------------
void Keyboard::pushEvent(int keyid, KeyState state)
{
    events.push_back(Event{keyid,state});
}
//--------------------------------------------------------------
bool Keyboard::pushEventIfPressed(KeyView& key, float x, float y, bool mouseDown)
{
    if(isOverKey(x,y,key))
    {
        key.color = ImGui::GetColorU32(ImVec4(0.5,0.5,0.5,1.0));
        if(mouseDown && key.id != _lastKeyPressed)
        {
            pushEvent(_lastKeyPressed, Released);
            pushEvent(key.id, Pressed);
            _lastKeyPressed = key.id;
        }
        return true;
    }
    return false;
}
//--------------------------------------------------------------
void Keyboard::processEvents()
{
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        pushEvent(_lastKeyPressed, Released);
        _lastKeyPressed = 0;
    }
    
    ImGuiIO& io = ImGui::GetIO();
    float mx = io.MousePos.x;
    float my = io.MousePos.y;
    bool mouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right);
    
    for(auto& kv : _blackKeys)
    {
        if (pushEventIfPressed(kv, mx, my, mouseDown))
            return;
    }
    for(auto& kv : _whiteKeys)
    {
        if (pushEventIfPressed(kv, mx, my, mouseDown))
            return;
    }
}
//--------------------------------------------------------------
void Keyboard::draw()
{
    float avail = ImGui::GetContentRegionAvail().x;
    float xOffset = ImGui::GetCursorScreenPos().x;
    float yOffset = ImGui::GetCursorScreenPos().y;
    
    _keyWidth = avail / (_keys.size() * 7.0/12.0);
    _keyHeight = 5.0 * _keyWidth;
    
    size_t bk_index = 0;
    size_t wk_index = 0;
    
    for(unsigned i = 0; i< _keys.size(); ++i)
    {
        unsigned io = (_keyFirstIndex+i-21) % 12;
        if(io == 1 || io == 4 || io == 6 || io == 9 || io == 11)
        {
            _blackKeys[bk_index].id = _keyFirstIndex + i;
            _blackKeys[bk_index].oft = xOffset - _keyWidth * 0.5;
            _blackKeys[bk_index].black = true;
            _blackKeys[bk_index].color = ImGui::GetColorU32(ImVec4(0.0,0.0,0.0,1.0));
            if(_keys[i]) _blackKeys[bk_index].color  = ImGui::GetColorU32(ImVec4(0.3,0.3,0.3,1.0));
            bk_index++;
        }
        else
        {
            _whiteKeys[wk_index].id = _keyFirstIndex + i;
            _whiteKeys[wk_index].oft = xOffset;
            _whiteKeys[bk_index].black = false;
            _whiteKeys[wk_index].color = ImGui::GetColorU32(ImVec4(1.0,1.0,1.0,1.0));
            if(_keys[i]) _whiteKeys[wk_index].color  = ImGui::GetColorU32(ImVec4(0.7,0.7,0.7,1.0));
            wk_index++;
            
            xOffset += _keyWidth;
        }
    }
    
    processEvents();
    
    auto imguiDrawList = ImGui::GetWindowDrawList();
    for(auto& kv : _whiteKeys)
    {
        imguiDrawList->AddRectFilled(ImVec2(kv.oft, yOffset), ImVec2(kv.oft + _keyWidth - 1, yOffset + _keyHeight), kv.color, 0.0f);
    }
    for(auto& kv : _blackKeys)
    {
        imguiDrawList->AddRectFilled(ImVec2(kv.oft, yOffset), ImVec2(kv.oft + _keyWidth - 1, yOffset + 0.6 * _keyHeight), kv.color, 0.0f);
    }
}
//--------------------------------------------------------------
void Keyboard::render()
{
    ImGui::SetNextWindowPos(ImVec2(90, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1230, 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Keyboard");
    draw();
    ImGui::End();
}
