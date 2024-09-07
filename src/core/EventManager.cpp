#include "EventManager.h"
#include <Arduino.h> // For millis()

EventManager::EventManager()
{
}

void EventManager::addEvent(const Event &event)
{
    if (event.timestamp - lastPressTimes[event.buttonID] > DEBOUNCE_DELAY)
    {
        eventQueue.push(event);
        lastPressTimes[event.buttonID] = event.timestamp; // Update last press time
    }
}

bool EventManager::getNextEvent(Event &event)
{
    if (!eventQueue.empty())
    {
        event = eventQueue.front();
        eventQueue.pop();
        return true;
    }
    return false;
}
