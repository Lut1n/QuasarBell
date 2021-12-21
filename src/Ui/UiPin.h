#ifndef UI_PIN_H
#define UI_PIN_H

#include <memory>

#include "Ui/UiSystem.h"
#include "Ui/UiRect.h"
#include "Ui/UiText.h"

using std::size_t;

namespace qb
{
    static inline bool hasFlag(size_t flagSet, size_t flag)
    {
        return (flagSet & flag) != 0;
    }
}

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
    bool isInput = false;
    bool rightClicking = false;

    // input flags
    // static constexpr size_t Type_None = 0;
    // static constexpr size_t Type_S1d = 0x1;
    // static constexpr size_t Type_S2d = 0x2;
    // static constexpr size_t Type_S3d = 0x4;
    // static constexpr size_t Type_S2d_3d = Type_S2d | Type_S3d;

    size_t typeFlags = 0;
};

#endif // UI_PIN_H
