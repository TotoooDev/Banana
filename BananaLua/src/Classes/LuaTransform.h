#pragma once
#include <Scene/Components.h>
#include <LuaClass.h>

namespace Banana
{
	class Vec3
	{
	public:
		static void Bind(lua_State* L)
		{
			LuaClass<Vec3> luaClass(L, "Vec3");
			luaClass.SetConstructor<float, float, float>();
			luaClass.SetFunction("GetX", &Vec3::GetX);
			luaClass.SetFunction("GetY", &Vec3::GetY);
			luaClass.SetFunction("GetZ", &Vec3::GetZ);
			luaClass.SetFunction("SetX", &Vec3::SetX);
			luaClass.SetFunction("SetY", &Vec3::SetY);
			luaClass.SetFunction("SetZ", &Vec3::SetZ);
			luaClass.SetFunction("__tostring", &Vec3::ToString);
		}

	public:
		Vec3() = default;

		Vec3(float x_, float y_, float z_)
			: x(x_), y(y_), z(z_)
		{}

		Vec3(glm::vec3 vec)
			: x(vec.x), y(vec.y), z(vec.z)
		{}

		float GetX() { return x; }
		float GetY() { return y; }
		float GetZ() { return z; }
		
		void SetX(float x_) { x = x_; }
		void SetY(float y_) { y = y_; }
		void SetZ(float z_) { z = z_; }

		std::string ToString()
		{
			return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
		}

	public:
		float x, y, z;
	};

	class LuaTransform
	{
	public:
		static void Bind(lua_State* L)
		{
			LuaClass<LuaTransform> luaClass(L, m_Name);
			luaClass.SetConstructor<>();
			luaClass.SetFunction("GetTranslation", &LuaTransform::GetTranslation);
			luaClass.SetFunction("GetRotation", &LuaTransform::GetTranslation);
			luaClass.SetFunction("GetScale", &LuaTransform::GetTranslation);
			luaClass.SetFunction("SetTranslation", &LuaTransform::SetTranslation);
			luaClass.SetFunction("SetRotation", &LuaTransform::SetRotation);
			luaClass.SetFunction("SetScale", &LuaTransform::SetScale);
		}

		static int GetComponent(lua_State* L)
		{
			lua_getglobal(L, "Entity");
			Entity* ent = (Entity*)lua_touserdata(L, -1);
			TransformComponent& comp = ent->GetComponent<TransformComponent>();
			LuaTransform transform(&comp);
			LuaTransform* transformUserdata = (LuaTransform*)lua_newuserdata(L, sizeof(LuaTransform));
			*transformUserdata = transform;
			luaL_newmetatable(L, typeid(LuaTransform).raw_name());
			lua_setmetatable(L, -2);
			return 1;
		}

		static std::string GetName()
		{
			return m_Name;
		}

	public:
		LuaTransform()
		{
			m_Component->Translation = glm::vec3(0.0f);
			m_Component->Rotation = glm::vec3(0.0f);
			m_Component->Scale = glm::vec3(1.0f);
		}

		LuaTransform(TransformComponent* comp)
			: m_Component(comp)
		{}

		TransformComponent* GetComponent() { return m_Component; }
		void SetComponent(const TransformComponent& comp) { *m_Component = comp; }

		Vec3 GetTranslation() { return m_Component->Translation; }
		Vec3 GetRotation() { return m_Component->Rotation; }
		Vec3 GetScale() { return m_Component->Scale; }

		void SetTranslation(Vec3 vec) { m_Component->Translation = glm::vec3(vec.x, vec.y, vec.z); }
		void SetRotation(Vec3 vec) { m_Component->Rotation = glm::vec3(vec.x, vec.y, vec.z); }
		void SetScale(Vec3 vec) { m_Component->Scale = glm::vec3(vec.x, vec.y, vec.z); }

	private:
		TransformComponent* m_Component;
		static constexpr std::string m_Name = "Transform";
	};
}