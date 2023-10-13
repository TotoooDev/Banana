#pragma once
#include <string>

namespace Banana
{
    /**
     * @brief The base event class.
     */
    class Event
    {
    public:
        /**
         * @returns The name of the event.
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
         * @brief Sets the event as handled.
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