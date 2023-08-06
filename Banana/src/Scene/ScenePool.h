#pragma once
#include <Core/Ref.h>
#include <Scene/Scene.h>
#include <map>
#include <string>

namespace Banana
{
	class ScenePool
	{
	public:
		void AddScene(Ref<Scene> scene, const std::string& name);
		Ref<Scene> LoadScene(const std::string& path, const std::string& name);
		Ref<Scene> GetSceneByName(const std::string& name);
		void DeleteScene(const std::string& name);

	private:
		std::map<std::string, Ref<Scene>> m_Scenes;
	};

	/*
	
	ScenePool scenePool;
	Ref<Scene> scene1 = scenePool.LoadScene("pathToScene1.scene", "scene1");
	Ref<Scene> scene2 = scenePool.LoadScene("pathToScene2.scene", "scene2");
	Application::Get()->SetCurrentScene(scene2);

	Ref<Scene> scene1Ref = scenePool.GetScene("scene1");
	Application::Get()->SetCurrentScene(scene1Ref);

	scenePool.UnloadScene("scene2"); // Once every reference the "scene2" is destroyed, the scene will be deleted from memory

	*/
}