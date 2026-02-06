#include "mesh.h"
#include "engine.h"

engine::Mesh::Mesh(const VertexLayout& vertexLayout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
{
	m_vertexLayout = vertexLayout;

	auto& graphicAPI = engine::Engine::GetInstance().GetGraphicAPI();

	m_VBO = graphicAPI.CreateVBO(vertices);
	m_EBO = graphicAPI.CreateEBO(indices);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	for (auto& element : m_vertexLayout.elements) {
		glVertexAttribPointer(element.attributeLocation, element.count, 
			element.dataType, GL_FALSE, m_vertexLayout.stride, (void*)(uintptr_t)(element.offset));
		glEnableVertexAttribArray(element.attributeLocation);

	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	m_indexCount = indices.size();
	m_vertexCount = vertices.size() * sizeof(float) /m_vertexLayout.stride;
}

engine::Mesh::Mesh(const VertexLayout& vertexLayout, const std::vector<float>& vertices)
{
	m_vertexLayout = vertexLayout;

	auto& graphicAPI = engine::Engine::GetInstance().GetGraphicAPI();

	m_VBO = graphicAPI.CreateVBO(vertices);


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);


	for (auto& element : m_vertexLayout.elements) {
		glVertexAttribPointer(element.attributeLocation, element.count,
			element.dataType, GL_FALSE, m_vertexLayout.stride, (void*)(uintptr_t)element.offset);
		glEnableVertexAttribArray(element.attributeLocation);

	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vertexCount = vertices.size() * sizeof(float) / (float)m_vertexLayout.stride;
}

void engine::Mesh::Bind()
{

	glBindVertexArray(m_VAO);
}

void engine::Mesh::Draw()
{
	if (m_indexCount > 0) {
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);


	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	}
}

void engine::Mesh::logInfo()
{
	spdlog::info("VAO IS {}, VBO IS {}, EBO IS{},index count is {},vertex count is {}", m_VAO, m_VBO, m_EBO, m_indexCount, m_vertexCount);
}
