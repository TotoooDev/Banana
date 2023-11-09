# Scripts

## What is a script?

In Banana, a script is a piece of code written in Lua that is interpreted and run by the engine. You can attach them to an entity to alter its behavior while the game is playing.

## Writing your first script

**Note: This is not a Lua tutorial! You can find a good introduction to the language in the [Programming in Lua](https://www.lua.org/pil/contents.html) book.**

First, create a lua script named `script.lua` in the `dev-assets` directory.

Every script in Banana **must** define two functions:

- `OnStart`, which is called when the script is created.
- `OnUpdate`, which is called every scene update.

Let's get scripting!

First, at the top of your script, write `Banana = require "Banana"`. This loads the Banana library into a table named `Banana`. This table is useful to call some Banana function, such as logging.

Then, write this piece of code:

```lua
function OnStart()
    Banana.LogInfo("Hello world!")
end
```

This defines the `OnStart` function that logs "Hello world!" to the console.

Next we will define the `OnUpdate` function:

```lua
function OnUpdate(timestep)
    -- Banana.LogInfo(timestep)
end
```

This function takes one argument `timestep` which is the amount of time in seconds that has passed since the last update. Here I commented the `LogInfo` call because I don't want to flood the console with messages.

Congratulations, you just created your first script!

## Attaching the script

Go back to your `ExampleScene.h` file where you define your entities. We will attach the script to the entity we created earlier, with as usual the `AddComponent` method:

```cpp
auto& sphereScript = newEntity.AddComponent<LuaScriptComponent>("script.lua");
```

We need to pass the name of our script `script.lua` in the constructor of the component.

And that's it! The script is attached to the entity and will be executed by Banana. If you run your game, you should see "Hello world!" printed to the console. Try uncommenting the line in `OnUpdate` to see your console flooded with log messages.

## Accessing components in scripts

Printing stuff to the console is cool and all, but it would be way cooler if you could access the components of your entity in your scripts. Fortunately, there is a way to do that!

We will write code to access and modify the position of our sphere entity. This is where the `Banana` table we declared earlier is useful, because we call the `Get<component>` function from there.

First, we need to get access to the transform component that defines the entity in space. This is done with a simple line of code:

```lua
transform = Banana.GetTransform()
```

The variable `transform` now holds a pointer to a structure containing all the useful information about the transform of our entity. We can access the translation with the `GetTranslation` method:

```lua
translation = transform:GetTranslation()
```

Here `translation` is a `Vec3`, a 3-dimensionnal vector.

Now we can move the entity with the `GetX` and `SetX` methods of `Vec3`:

```lua
x = translation:GetX()
x += 5 * timestep -- multiply by the timestep to have a constant speed across framerates
translation:SetX(x)
```

This is what your `OnUpdate` function should look like now:

```lua
function OnUpdate(timestep)
    -- Banana.LogInfo(timestep)
    transform = Banana.GetTransform()
    translation = transform:GetTranslation()
    x = translation:GetX()
    x += 5 * timestep -- multiply by the timestep to have a constant speed across framerates
    translation:SetX(x)
end
```

If you run your game now, you should see your entity moving!

By the way, this whole function could be reduced to a single line of code: `Banana.GetTransform():GetTranslation():SetX(Banana.GetTransform():GetTranslation():GetX() + 5 * timestep)` which is really ugly (but it works lol).

---

You're now ready to read the [next section](custom_components.md).
