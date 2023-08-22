#include <Graphics/Primitives/Plane.h>

namespace Banana
{
	Plane::Plane(unsigned int resX, unsigned int resZ)
	{
		if (resX == 0 || resZ == 0)
			return;

		unsigned int indexCount = 6 * resX * resZ;
		unsigned int vertexCount = (resX + 1) * (resZ + 1);

		std::vector<unsigned int> indices(indexCount);
		std::vector<glm::vec3> vertices(vertexCount);
		std::vector<glm::vec2> uvs(vertexCount);

		// Vertices
		float xStep = 2.0f / resX; // geometry dash reference??????
		float zStep = 2.0f / resZ;

		float uStep = 1.0f / resX;
		float vStep = 1.0f / resZ;

		glm::vec3 offset(-1.0f, 0.0f, 1.0f);

		for (unsigned int x = 0; x <= resX; x++)
		{
			for (unsigned int z = 0; z <= resZ; z++)
			{
				unsigned int i = z + x * (resZ + 1);

				vertices[i] = glm::vec3(x * xStep, 0.0f, z * zStep) + offset;
				uvs[i] = glm::vec2(x * uStep, (resZ - z) * vStep);
			}
		}

		for (unsigned int v = 0, i = 0; v < vertexCount - resZ - 2; v++)
		{
			if ((v + 1) % (resZ + 1) == 0)
				v++;

			indices[i++] = v;
			indices[i++] = v + 1;
			indices[i++] = v + resZ + 1;

			indices[i++] = v + 1;
			indices[i++] = v + resZ + 2;
			indices[i++] = v + resZ + 1;
		}

		// VAO creation
		VertexLayout layout;
		layout.AddAttribute(Type::Vec3); // Pos
		layout.AddAttribute(Type::Vec2); // UV

		m_VAO = VAO::Create(layout, vertexCount);
		m_VAO->SetData(0, vertices);
		m_VAO->SetData(1, uvs);

		m_EBO = EBO::Create();
		m_EBO->SetData(indices);
	}
}