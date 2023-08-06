#include <Scene/ScenePool.h>

namespace Banana
{
	void ScenePool::AddScene(Ref<Scene> scene, const std::string& name)
	{
		m_Scenes[name] = scene;
	}

	Ref<Scene> ScenePool::LoadScene(const std::string& path, const std::string& name)
	{
		// Do some cool loading stuff here
		return nullptr;
	}

	Ref<Scene> ScenePool::GetSceneByName(const std::string& name)
	{
		return m_Scenes[name];
	}

	void ScenePool::DeleteScene(const std::string& name)
	{
		m_Scenes[name]->OnDelete();
		m_Scenes.erase(name);
	}
}