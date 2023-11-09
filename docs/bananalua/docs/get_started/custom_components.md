# Custom components

Using the Banana-defined components in your can be enough for your game, but at some point your will need to define your own components and bind them to Lua. This is actually very straight-forward, thanks to the BananaLua API (that took me 2 weeks to implement and I'm sure there are still some bugs I hate my life).

## Define a new component

This is the simplest part: any data structure (`struct` and `class` are the ones you will usually use) can be a component. So to define a new component you just need to define a new class or structure.

Create a file named `MyComponent.h` and define a new class in it:

```cpp
#include <Core/Log.h>

class MyComponent
{
public:
    MyComponent(const std::string& message)
        : m_Message(message)
    {}

    void PrintMessage()
    {
        BANANA_INFO(m_Message);
    }

    std::string GetMessage()
    {
        return m_Message;
    }

private:
    std::string m_Message;
}
```

Don't forget to regenerate your project by running the `GenerateProject` script!

As you can see, this is a really simple class in C++. Attach it to your entity:

```cpp
auto& customComponent = newEntity.AddComponent<MyComponent>("Some cool message");
```

That was the simple part, now we are going to bind the class to Lua.

## Lua bindings

For this, we will create a static method in `MyComponent` called `Bind`. This is where we will send Lua the information about the class.

First, include the necessary file at the top:

```cpp
#include <LuaClass.h>
```

Then, the static method:

```cpp
static void Bind(lua_State* L)
{
    LuaClass<MyComponent> luaClass(L, "MyComponent");
    luaClass.SetConstructor<std::string>();
    luaClass.SetFunction("PrintMessage", &MyComponent::PrintMessage);
    luaClass.SetFunction("GetMessage", &MyComponent::GetMessage);
}
```

There is a lot to unpack here, so we will do it in order:

- The argument of the method `lua_State* L` is the global state of your script. It's used to define globals, functions and tables inside of Lua. Usually you we not interact with it directly.
- First, we define a variable `luaClass` of type `LuaClass<MyComponent>`. We pass in the constructor the lua state `L`, and the name of the component that is used in the script.
- Then we set a constructor with the `SetConstructor` method. In the template arguments, we pass the type of the arguments used in the constructor. It can take an argument that is the name of the constructor, which is by default `new`.
- Finally we set the methods of the class with the `SetFunction` method. The first argument is the name of the method in Lua, and the second is the actual function.

Now we just need to call this static function in our scene:

```cpp
MyComponent::Bind(sphereScript.GetState());
```

And that's all!

## Using the component

Back in Lua, using a custom component is really easy. All you need is to use it like every other component:

```lua
myComponent = MyComponent.new("Cool message")
message = myComponent:GetMessage()
myComponent:PrintMessage()
```

---

New features are on their way, stay tuned for more!
