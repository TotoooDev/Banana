#pragma once

#include <string>

namespace Banana
{
    // Base event class
    class Event
    {
    public:
        std::string GetName()
        {
            return m_Name;
        }

        bool IsHandled()
        {
            return m_Handled;
        }

        void SetHandled()
        {
            m_Handled = true;
        }

    protected:
        std::string m_Name = "Unnamed Event";
        bool m_Handled = false;
    };
}