#pragma once

#include <Events/Event.h>

#include <list>
#include <map>
#include <typeindex>

namespace Banana
{
    // From https://medium.com/@savas/nomad-game-engine-part-7-the-event-system-45a809ccb68f

    class HandlerFunctionBase
    {
    public:
        // Call the member function
        void Execute(Event* event)
        {
            Call(event);
        }

    private:
        // Implemented by MemberFunctionHandler
        virtual void Call(Event* event) = 0;
    };

    template <class T, class EventType>
    class MemberFunctionHandler : public HandlerFunctionBase
    {
    public:
        typedef void (T::* MemberFunction)(EventType*);

        MemberFunctionHandler(T* instance, MemberFunction memberFunction)
            : m_Instance(instance), m_MemberFunction(memberFunction)
        {

        }

        void Call(Event* event) override
        {
            // Cast event to the correct type and call member function
            if (!event->IsHandled())
                (m_Instance->*m_MemberFunction)(static_cast<EventType*>(event));
        }

    private:
        // Pointer to the class instance
        T* m_Instance;
        // Pointer to the memeber function
        MemberFunction m_MemberFunction;
    };

    typedef std::list<HandlerFunctionBase*> HandlerList;
    class EventBus
    {
    public:
        template <typename EventType>
        void Publish(EventType* event)
        {
            HandlerList* handlers = m_Subscribers[typeid(EventType)];

            if (handlers == nullptr)
            {
                return;
            }

            for (auto& handler : *handlers)
            {
                if (handler != nullptr)
                {
                    handler->Execute(event);
                }
            }
        }

        template <class T, class EventType>
        void Subscribe(T* instance, void (T::* MemberFunction)(EventType*))
        {
            HandlerList* handlers = m_Subscribers[typeid(EventType)];

            // First time initialization
            if (handlers == nullptr)
            {
                handlers = new HandlerList();
                m_Subscribers[typeid(EventType)] = handlers;
            }

            handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, MemberFunction));
        }

    private:
        std::map<std::type_index, HandlerList*> m_Subscribers;
    };
}