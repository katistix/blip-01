#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include <queue>
#include <cstdint>

enum class EventType
{
    BUTTON_PRESS
};

struct Event
{
    EventType type;
    int buttonID;
    uint32_t timestamp;

    Event(EventType t, int id, uint32_t time) : type(t), buttonID(id), timestamp(time) {}
};

class EventManager
{
public:
    EventManager();

    void addEvent(const Event &event);
    bool getNextEvent(Event &event);

private:
    static const uint32_t DEBOUNCE_DELAY = 200; // Debounce delay in milliseconds
    uint32_t lastPressTimes[4] = {0, 0, 0, 0};  // Track the last press time for each button
    std::queue<Event> eventQueue;
};

#endif // EVENT_MANAGER_H