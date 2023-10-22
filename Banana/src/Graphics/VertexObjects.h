#pragma once
#include <Core/Ref.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace Banana
{
	/**
	 * Represents a type to use in a VertexObject because I was too lazy to work with templates.
	 */
	enum class Type
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat4
	};

	/**
	 * Represents an attribute in the VertexLayout.
	 */
	struct Attribute
	{
		Type DataType;
		unsigned int AttributeIndex;
		unsigned int Size;
		unsigned int NumData; // 1 for Float, 2 for Vec2, ...
	};

	/**
	 * Specifies the layout of the vertex data in memory.
	 * WARNING: Most of the instance methods are unfinished and do not work as of now!
	 */
	class VertexLayout
	{
	public:
		/**
		 * Adds an attribute to the layout.
		 * @param type - The type of the attribute.
		 */
		void AddAttribute(Type type);

		void AddAttributeInstance(Type type, unsigned int num);

	// Getters
	public:
		/**
		 * @returns A vector of the atributes in the layout.
		 */
		std::vector<Attribute> GetAttributes() const { return m_Attributes; }

		std::vector<Attribute> GetAttributesInstance() const { return m_AttributesInstance; }

		/**
		 * @returns The size in bytes of a single vertex using this layout.
		 */
		unsigned int GetVertexSize() const { return m_VertexSize; }

		unsigned int GetInstanceSize() const { return m_InstanceSize; }
		
	private:
		std::vector<Attribute> m_Attributes;
		std::vector<Attribute> m_AttributesInstance;
		unsigned int m_VertexSize = 0;
		unsigned int m_InstanceSize = 0;
		unsigned int m_AttributeIndex = 0;
	};

	/**
	 * Provides an API for Vertex Buffer Objects
	 */
	class VAO
	{
	public:
		/**
		 * @returns The newly created VAO.
		 * @param layout - The layout to use for this VAO.
		 * @param numVertices - The number of vertices in the VAO.
		 */
		static Ref<VAO> Create(const VertexLayout& layout, unsigned int numVertices);

		/**
		 * Binds the VAO for future uses.
		 */
		virtual void Bind() = 0;

		/**
		 * @param attributeIndex - The index of the attribute in the layout.
		 * @param data - The values to set the attribute to.
		 */
		virtual void SetData(unsigned int attributeIndex, std::vector<float> data) = 0;
		/**
		 * @param attributeIndex - The index of the attribute in the layout.
		 * @param data - The values to set the attribute to.
		 */
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec2> data) = 0;
		/**
		 * @param attributeIndex - The index of the attribute in the layout.
		 * @param data - The values to set the attribute to.
		 */
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec3> data) = 0;
		/**
		 * @param attributeIndex - The index of the attribute in the layout.
		 * @param data - The values to set the attribute to.
		 */
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec4> data) = 0;
		/**
		 * @param attributeIndex - The index of the attribute in the layout.
		 * @param data - The values to set the attribute to.
		 */
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::mat4> data) = 0;

		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<float> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec2> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec3> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec4> data) = 0;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::mat4> data) = 0;
		
		VertexLayout* GetLayout() { return &m_Layout; }

	protected:
		unsigned int m_NumVertices;
		VertexLayout m_Layout;
	};

	/**
	 * Provides an API for Element Buffer Objects.
	 */
	class EBO
	{
	public:
		/**
		 * @returns The newly created EBO;
		 */
		static Ref<EBO> Create();

		/**
		 * Binds the EBO for future uses.
		 */
		virtual void Bind() = 0;

		/**
		 * @param indices - The indices values.
		 */
		virtual void SetData(const std::vector<unsigned int>& indices) = 0;

		/**
		 * @returns The number of indices in the EBO.
		 */
		unsigned int GetCount() const { return m_Count; }
		
	protected:
		unsigned int m_Count = 0;
	};
}