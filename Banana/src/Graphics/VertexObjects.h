#pragma once
#include <Core/Ref.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
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
		// WARNING: This is unfinished
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
		static Ref<VAO> Create(const VertexLayout& layout, unsigned int numVertices);

		virtual void SetData(unsigned int attributeIndex, std::vector<float> data) = 0;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec2> data) = 0;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec3> data) = 0;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec4> data) = 0;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::mat4> data) = 0;

		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<float> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec2> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec3> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec4> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::mat4> data) = 0;

		virtual void Bind() = 0;
		
		VertexLayout* GetLayout() { return &m_Layout; }

	protected:
		unsigned int m_NumVertices;
		VertexLayout m_Layout;
	};

	class EBO
	{
	public:
		static Ref<EBO> Create();

		virtual void SetData(const std::vector<unsigned int>& indices) = 0;
		virtual void Bind() = 0;

		unsigned int GetCount() const { return m_Count; }
		
	protected:
		unsigned int m_Count = 0;
	};
}