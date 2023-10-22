#pragma once
#include <string>

namespace Banana
{
    /**
     * The base event class. All events published by the EventBus must inherit from this class.
     */
    class Event
    {
    public:
        /**
         * The name of the event.
         */
        std::string GetName()
        {
            return m_Name;
        }

        /**
         * @returns Whether the event is handled or not.
         */
        bool IsHandled()
        {
            return m_Handled;
        }

        /**
         * Sets the event as handled.
         */
        void SetHandled()
        {
            m_Handled = true;
        }

    protected:
        std::string m_Name = "Unnamed Event";
        bool m_Handled = false;
    };
}