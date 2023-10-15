#include <Graphics/Log.h>
#include <Graphics/VertexObjects.h>
#include <Graphics/RendererAPI.h>
#include <Graphics/OpenGL/OpenGLVertexObjects.h>

namespace Banana
{
	void VertexLayout::AddAttribute(Type type)
	{
		unsigned int size = 0;
		unsigned int numData = 0;
		switch (type)
		{
		case Type::Float: size = sizeof(float); numData = 1; break;
		case Type::Vec2: size = sizeof(float) * 2; numData = 2; break;
		case Type::Vec3: size = sizeof(float) * 3; numData = 3; break;
		case Type::Vec4: size = sizeof(float) * 4; numData = 4; break;
		case Type::Mat4: size = sizeof(float) * 16; numData = 16; break;
		}

		Attribute attribute = { type, m_AttributeIndex, size, numData };
		m_Attributes.push_back(attribute);

		m_VertexSize += size;

		m_AttributeIndex++;
		// If the attribute is a mat4 (which is basically 4 vec4s) we need to increment the attribute index by 4
		if (type == Type::Mat4)
			m_AttributeIndex += 3;
	}

	void VertexLayout::AddAttributeInstance(Type type, unsigned int num)
	{
		unsigned int size = 0;
		unsigned int numData = 0;
		switch (type)
		{
		case Type::Float: size = sizeof(float); numData = 1; break;
		case Type::Vec2: size = sizeof(float) * 2; numData = 2; break;
		case Type::Vec3: size = sizeof(float) * 3; numData = 3; break;
		case Type::Vec4: size = sizeof(float) * 4; numData = 4; break;
		case Type::Mat4: size = sizeof(float) * 16; numData = 16; break;
		}

		Attribute attribute = { type, m_AttributeIndex, size, numData };
		m_AttributesInstance.push_back(attribute);

		m_VertexSize += size;

		m_AttributeIndex++;
		// If the attribute is a mat4 (which is basically 4 vec4s) we need to increment the attribute index by 4
		if (type == Type::Mat4)
			m_AttributeIndex += 3;
	}

	Ref<VAO> VAO::Create(const VertexLayout& layout, unsigned int numVertices)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIName::OpenGL:
			return CreateRef<OpenGLVAO>(layout, numVertices);
		case RendererAPIName::Vulkan:
			BANANA_ABORT("Vulkan is not supported yet :(");
		case RendererAPIName::None:
		default:
			BANANA_ABORT("Unsupported renderer API {}!", (int)RendererAPI::GetAPI());
		}

		return nullptr;
	}

	Ref<EBO> EBO::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIName::OpenGL:
			return CreateRef<OpenGLEBO>();
		case RendererAPIName::Vulkan:
			BANANA_ABORT("Vulkan is not supported yet :(");
		case RendererAPIName::None:
		default:
			BANANA_ABORT("Unsupported renderer API {}!", (int)RendererAPI::GetAPI());
		}

		return nullptr;
	}
}