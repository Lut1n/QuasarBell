#ifndef UI_PIN_H
#define UI_PIN_H

#include <memory>

#include "UI/UiSystem.h"
#include "UI/UiRect.h"
#include "UI/UiText.h"

struct UiNode;

struct UiPin : public UiRect
{
    UiPin(UiNode* node, const std::string& label, const vec2& position, const vec2& size);
    ~UiPin() override;
    
    bool onEvent(const UiEvent& event) override;
    void draw() override;
    
    void addConnectionId(std::uint64_t id);
    void remConnectionId(std::uint64_t id);

    UiPin* getLinked();

public:
    std::unique_ptr<UiText> label;
    UiNode* parentNode = nullptr;
    std::vector<std::uint64_t> connectionIds;
    bool multipleConnections = false;
    bool textOnLeft = false;

    enum Type
    {
        TYPE_BOOL,
        TYPE_INT1,
        TYPE_INT2,
        TYPE_INT3,
        TYPE_INT4,
        TYPE_FLOAT1,
        TYPE_FLOAT2,
        TYPE_FLOAT3,
        TYPE_FLOAT4,
        TYPE_MATRIX3,
        TYPE_MATRIX4,
        TYPE_TEXTURE,
        TYPE_GEOMETRY,
        TYPE_TEXT,
        TYPE_PCM,

        TYPE_COUNT
    };

    Type type = TYPE_BOOL;
};

#endif // UI_PIN_H
