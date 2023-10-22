#pragma once
#include <Events/Event.h>
#include <list>
#include <map>
#include <typeindex>

namespace Banana
{
    // Stolen from https://medium.com/@savas/nomad-game-engine-part-7-the-event-system-45a809ccb68f.
    // I'll be honest, I don't understand how all of this work in details, I just know how to used it.
    // That's why there is almost no documentation in this file lol.

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
    /**
     * The class that distributes all the published events to all the subscribers. You can subscribe a class to an event and the member function you passed when subscribing will get called when the event is published.
     * You are not limited to the base Banana events! You can absolutely create your own event by making them inherit the Event class.
     */
    class EventBus
    {
    public:
        /**
         * Publishes an event. All the classes subscribed to this event will get their member function called.
         * @param event - The event that gets published.
         */
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

        /**
         * Subscribe a class to an event. When this event is published, the member function passed in parameter will get called.
         * @param instance - The instance of the class to subscribe.
         * @param MemberFunction - The member function that will get called when the event is published.
         */
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