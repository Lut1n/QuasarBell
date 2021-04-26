#ifndef UI_CONNECTIONS_H
#define UI_CONNECTIONS_H

#include <memory>
#include <unordered_map>

#include "UI/UiSystem.h"
#include "UI/UiRect.h"
#include "UI/UiText.h"

struct UiPin;

struct UiConnections : public UiElement
{
    struct Handler
    {
        virtual void onConnect(UiPin* a, UiPin* b) = 0;
        virtual void onDisconnect(UiPin* a, UiPin* b) = 0;
    };

    UiConnections();
    
    bool onEvent(const UiEvent& event) override;
    void update(const UiTime& time) override;
    void draw() override;
    Rect computeBounds() const override;

    std::uint64_t createLink(UiPin* a, UiPin* b);
    void updateLink(std::uint64_t id, UiPin* a, UiPin* b);
    void deleteLink(std::uint64_t id);

    std::uint64_t startLink(UiPin* a);
    void updateCurrentLink(vec2 b);
    std::uint64_t endLink(UiPin* b);
    void abortLink();

    void drawLink(vec2 p1, vec2 p2);
    
public:

    static UiConnections* instance;

    using Link = std::pair<UiPin*, UiPin*>;

    std::pair<UiPin*, vec2> partialLink;
    bool displayPartial = false;
    std::uint64_t currentId = 0;
    std::uint64_t previousId = 1;
    std::unordered_map<std::uint64_t,Link> links;
    Handler* handler = nullptr;
};

#endif // UI_CONNECTIONS_H
