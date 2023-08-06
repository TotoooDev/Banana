#pragma once

#include <vector>
#include <spdlog/spdlog.h>
#include <GL/glew.h>

namespace Banana
{
	enum class Type
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat4,
	};

	class VertexLayout
	{
	public:
		struct Attribute
		{
			Banana::Type Type;
			int Offset;
		};

		void AddAttribute(Type type);
		std::vector<Attribute> GetAttributes() const { return m_Attributes; }
		unsigned int GetStride() const { return m_Stride; }
		unsigned int GetTotalCount() const { return m_TotalCount; }

	private:
		std::vector<Attribute> m_Attributes;
		unsigned int m_Stride = 0;
		unsigned int m_TotalCount = 0;
	};

	class VAO
	{
	public:
		VAO();
		~VAO();

		template <typename T>
		void SetData(const std::vector<T>& data, const VertexLayout& layout)
		{
			SetData((void*)data.data(), (int)(data.size() * sizeof(T)), layout);
			m_VertexCount += (unsigned int)data.size() / layout.GetTotalCount();
		}
		void SetData(void* data, int size, const VertexLayout& layout);

		void Bind() const;
		unsigned int GetID() const { return m_ID; }
		unsigned int GetVertexCount() const { return m_VertexCount; }

	private:
		unsigned int m_ID = 0;
		unsigned int m_VBO = 0;
		unsigned int m_VertexCount = 0;
		unsigned int m_VertexAttribLocation = 0;
	};

	class EBO
	{
	public:
		EBO();
		~EBO();

		void SetData(const std::vector<unsigned int>& indices);
		void Bind() const;
		unsigned int GetID() const { return m_ID; }
		unsigned int GetCount() const { return m_Count; }

	private:
		unsigned int m_ID = 0;
		unsigned int m_Count = 0;
	};
}