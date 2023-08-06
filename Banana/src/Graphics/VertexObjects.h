#pragma once
#include <GL/glew.h>
#include <vector>

namespace Banana
{
	/*
	VertexLayout layout;
	layout.AddAttribute(Vec3); // Position
	layout.AddAttribute(Vec2); // Texture coords
	layout.AddAttributeInstance(Mat4, numInstances); // Model matrix of each instance

	VAO vao(layout);
	vao.SetData(0, data); // Sets attribute 0 (here the Positions) to data
	vao.SetData(1, data); // Sets attribute 0 (here the Texture coords) to data
	vao.SetDataInstance(0, data);
	*/

	enum class Type
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat4
	};

	struct Attribute
	{
		Type DataType;
		unsigned int AttributeIndex;
		unsigned int Size;
		unsigned int NumData; // 1 for Float, 2 for Vec2, ...
	};

	// Specifies the layout of the vertex data in memory
	class VertexLayout
	{
	public:
		void AddAttribute(Type type);
		void AddAttributeInstance(Type type, unsigned int num);

		std::vector<Attribute> GetAttributes() const { return m_Attributes; }
		std::vector<Attribute> GetAttributesInstance() const { return m_AttributesInstance; }
		unsigned int GetVertexSize() const { return m_VertexSize; }
		unsigned int GetInstanceSize() const { return m_InstanceSize; }
		
	private:
		std::vector<Attribute> m_Attributes;
		std::vector<Attribute> m_AttributesInstance;
		unsigned int m_VertexSize = 0;
		unsigned int m_InstanceSize = 0;
		unsigned int m_AttributeIndex = 0;
	};

	// OpenGL Vertex Array Object wrapper class
	// You need to create a VertexLayout first to define the layout of your vertex data
	class VAO
	{
	public:
		VAO(const VertexLayout& layout, unsigned int numVertices);
		~VAO();

		template <typename T>
		void SetData(unsigned int attributeIndex, std::vector<T> data)
		{
			Attribute attribute = m_Layout.GetAttributes()[attributeIndex];
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, attribute.AttributeIndex * attribute.Size * m_NumVertices, data.size() * attribute.NumData, data.data());
		}
		
		template <typename T>
		void SetDataInstance(unsigned int attributeIndex, std::vector<T> data)
		{
			// TODO: Set instance data
		}

		void Bind();
		
		VertexLayout* GetLayout() { return &m_Layout; }
		unsigned int GetID() const { return m_VAO; }
		unsigned int GetVBO() const { return m_VBO; }
		unsigned int GetInstanceVBO() const { return m_InstanceVBO; }

	private:
		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;
		unsigned int m_InstanceVBO = 0;

		unsigned int m_AttributeIndex = 0;
		unsigned int m_NumVertices;
		VertexLayout m_Layout;
	};

	class EBO
	{
	public:
		EBO();
		~EBO();

		void SetData(const std::vector<unsigned int>& indices);
		unsigned int GetID() const { return m_ID; }
		unsigned int GetCount() const { return m_Count; }
		void Bind() const;

	private:
		unsigned int m_ID = 0;
		unsigned int m_Count = 0;
	};
}