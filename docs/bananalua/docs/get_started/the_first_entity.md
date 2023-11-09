# Your first entity

## What is an entity?

An entity is basically anything in the engine. Your character is an entity, the camera is an entity, even the skybox is represented as an entity.

You can attach components to your entities to alter their behavior. For example, the transform component defines the position, rotation and scale in space of your entity.

Any data structure can become a component, that means you can define your own components!

## What is a scene?

A scene is a container for entities. Every game loop, it executes some operations on the entities depending on their components. For example, for each entity that has the Lua script component, it will execute the `OnUpdate` function of that script.

Every scene has multiple entities, and you can have multiple scene in your project. We will soon see how to change between scenes.

Every scene you create must inherit from the `Banana::Scene` class.

## How to create an entity

Once you've opened the project, open the `ExampleScene.h` file in the `GameProject` folder. It contains a simple example of what a scene may look like.

We will modify this scene to render a sphere to the screen. We will need a few things:

- An entity with a transform in space and a sphere model.
- Another entity with a transform in space and a camera.

### The sphere entity

Let's create the first entity:

```cpp
Banana::Entity newEntity = CreateEntity("cool entity");
```

The `Scene::CreateEntity` method create a new entity with a name in parameter. Every game loop, the new entity will be updated corresponding to its components.

Let's add the components to our entity!

```cpp
auto& sphereTransform = newEntity.AddComponent<Banana::TransformComponent>();
auto& sphereMesh = newEntity.AddComponent<Banana::MeshComponent>(Banana::CreateRef<Banana::Icosphere>(3));
```

This attaches a transform component and a mesh component to the new entity. In the mesh component, we pass a reference to an icosphere mesh with 3 subdivisions.

The components we just created are stored in the `sphereTransform` and `sphereMesh` variables. Since those variables are references, changing their value will directly impact the component of the entity.

To see this in action, let's modify the scale of the sphere:

```cpp
sphereTransform.Translation = glm::vec3(0.5f);
```

This line of code will half the size of the sphere.

### The camera entity

In order for the renderer to render a scene, we need to add at least one camera component into the world. We will create a new entity for this.

Let's create our camera entity:

```cpp
Banana::Entity cam = CreateEntity("camera");
auto& camTransform = cam.AddComponent<Banana::TransformComponent>();
auto& camCam = cam.AddComponent<Banana::CameraComponent>(Banana::CreateRef<Banana::Camera>());
```

Just like before, we create a new entity and attach the needed components.

You can now build and launch your game, and see a beautiful half-sized pink sphere on the screen! Try to play with different parameters to see how they interact.

Next we will see what are scripts and how to use them.

---

You're now ready to read the [next section](scripts.md).
