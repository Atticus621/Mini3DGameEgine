#include "mesh.h"
#include "engine.h"


#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

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

std::shared_ptr<engine::Mesh> engine::Mesh::Load(const std::string& filePath)
{
	auto contents = engine::Engine::GetInstance().GetFileSystem().LoadAssetsFileText(filePath);    
	if (contents.empty())
	{
		spdlog::error("Failed to load mesh file: {}", filePath);
		return nullptr;
	}
	cgltf_options options = {};
	cgltf_data* data = nullptr;
	cgltf_result result = cgltf_parse(&options, contents.data(), contents.size(), &data);
	spdlog::error("here");
	if (result != cgltf_result_success)
	{
		spdlog::error("Failed to parse mesh file: {}, error code: {}", filePath, static_cast<int>(result));
		return nullptr;
	}
	auto binPath = engine::Engine::GetInstance().GetFileSystem().GetAssetsPath() / filePath;
	result = cgltf_load_buffers(&options, data, binPath.remove_filename().string().c_str());       
	if(result != cgltf_result_success)
	{
		spdlog::error("Failed to load buffers for mesh file: {}, error code: {}", filePath, static_cast<int>(result));
		cgltf_free(data);
		return nullptr;
	}

	auto readFloats = [](const cgltf_accessor* accessor, cgltf_size i, float* out, int n) {
		std::fill(out, out + n, 0.0f);
		return cgltf_accessor_read_float(accessor, i, out, n) == 1;
		};
	auto readIndex = [](const cgltf_accessor* accessor, cgltf_size i) {
		cgltf_uint out = 0;
		cgltf_bool ok = cgltf_accessor_read_uint(accessor, i, &out, 1);
		return ok ? static_cast<uint32_t>(out) : 0;
		};
	std::shared_ptr<Mesh> mesh = nullptr;
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	for (cgltf_size i = 0; i < data->meshes_count; ++i)
	{
		cgltf_mesh& cgltfMesh = data->meshes[i];
		for (cgltf_size j = 0; j < cgltfMesh.primitives_count; ++j)
		{
			cgltf_primitive& primitive = cgltfMesh.primitives[j];                  if (primitive.type != cgltf_primitive_type_triangles)
			{
				spdlog::warn("Mesh primitive type is not triangles, skipping");
				continue;
			}

			VertexLayout vertexLayout;
			cgltf_accessor* accessors[3] = { nullptr, nullptr, nullptr };
			for (cgltf_size k = 0; k < primitive.attributes_count; ++k)
			{
				cgltf_attribute& attribute = primitive.attributes[k];
				auto acc = attribute.data;
				if (!acc)
				{
					spdlog::warn("Mesh attribute has no accessor, skipping");
					continue;
				}

				VertexElemnet element;
				element.dataType = GL_FLOAT;
				switch (attribute.type)
				{
				case cgltf_attribute_type_position:
				{
					accessors[engine::VertexElemnet::PositionAttributeLocation] = acc;
					element.attributeLocation = engine::VertexElemnet::PositionAttributeLocation;
					element.count = 3;
				}
				break;
				case cgltf_attribute_type_color:
				{
					if (attribute.index != 0)
					{
						spdlog::warn("Mesh color attribute has index {}, only index 0 is supported, skipping", attribute.index);
						continue;
					}
					accessors[engine::VertexElemnet::ColorAttributeLocation] = acc;
					element.attributeLocation = engine::VertexElemnet::ColorAttributeLocation;
					element.count = 4;
				}
				break;
				case cgltf_attribute_type_texcoord:
				{
					if (attribute.index != 0)
					{
						spdlog::warn("Mesh color attribute has index {}, only index 0 is supported, skipping", attribute.index);
						continue;
					}
					accessors[engine::VertexElemnet::UVAttributeLocation] = acc;
					element.attributeLocation = engine::VertexElemnet::UVAttributeLocation;
					element.count = 2;
				}
				
				break;
				default:
					spdlog::warn("Mesh attribute type is not supported, skipping");
					continue;
				}

				if (element.count > 0) {
					element.offset = vertexLayout.stride;
					vertexLayout.stride += element.count * sizeof(float);
					vertexLayout.elements.push_back(element);
				}

			}   if(!accessors[engine::VertexElemnet::PositionAttributeLocation])
			{
				spdlog::warn("Mesh primitive has no position attribute, skipping");
				continue;
			}
			auto vertexCount = accessors[engine::VertexElemnet::PositionAttributeLocation]->count;
			vertices.resize(vertexCount* vertexLayout.stride / sizeof(float));
			for (size_t v = 0; v < vertexCount; ++v)
			{
				float* vertexPtr = vertices.data() + v * vertexLayout.stride / sizeof(float);
				for (auto& element : vertexLayout.elements) {
					if (accessors[element.attributeLocation]) {
						readFloats(accessors[element.attributeLocation], v, vertexPtr + element.offset / sizeof(float), element.count);
					}
				}
			}
			if(primitive.indices)
			{
				auto indexCount = primitive.indices->count;
				indices.resize(indexCount);
				for (size_t idx = 0; idx < indexCount; ++idx)
				{
					indices[idx] = readIndex(primitive.indices, idx);
				}
				mesh = std::make_shared<Mesh>(vertexLayout, vertices, indices);
			}
			else {
				mesh = std::make_shared<Mesh>(vertexLayout, vertices);
			}

			if (mesh) {
				break;
			}
		
		}
		if (mesh)
		{
			break;
		}
	}
	
	cgltf_free(data);
	return mesh;
	
}

void engine::Mesh::logInfo()
{
	spdlog::info("VAO IS {}, VBO IS {}, EBO IS{},index count is {},vertex count is {}", m_VAO, m_VBO, m_EBO, m_indexCount, m_vertexCount);
}
