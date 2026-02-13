#include "game_object.h"

#include "render/material.h"
#include "graphic/texture.h"
#include "scene/components/mesh_component.h"
#include "scene/components/animation_component.h"
#include "render/mesh.h"

#ifndef CGLTF_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#endif
#include <cgltf.h>
#include "engine.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif 
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphic/vertex_layout.h>
void engine::GameObject::Update(float delta)
{
	if (!m_isActive)return;
    for (auto& component : m_components) {
		component->Update(delta);
    }
    for(auto& child:m_children) {
		if (child->IsAlive()) {
           child->Update(delta);
       }
        else{
           m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
                [](const std::unique_ptr<GameObject>& child) { return !child->IsAlive(); }),
				m_children.end());
        }
	}

	
}

const std::string& engine::GameObject::GetName() const
{
	return m_name;
}

void engine::GameObject::SetName(const std::string& name)
{
	m_name = name;
}

void engine::GameObject::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

void engine::GameObject::SetRotation(const glm::quat& rotation)
{
	m_rotation = rotation;
}

void engine::GameObject::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
}

void engine::GameObject::AddComponent(engine::Component* component)
{
	m_components.emplace_back(component);
	component->m_owner = this;
}

bool engine::GameObject::IsAlive() const
{
    return m_isAlive;
}

bool engine::GameObject::IsActive() const
{
	return m_isActive;
}

glm::mat4 engine::GameObject::GetLocalTransform() const
{
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), m_position);

	mat *= glm::mat4_cast(m_rotation);

	mat = glm::scale(mat, m_scale);

	return mat;
}

glm::mat4 engine::GameObject::GetWorldTransform() const
{
    if (m_parent) {
		return m_parent->GetWorldTransform() * GetLocalTransform();
    }
    else {
		return GetLocalTransform();
    }
}
// 读取单个标量（float）
auto ReadScalar = [](cgltf_accessor* acc, cgltf_size index)
	{
		float res = 0.0f;
		cgltf_accessor_read_float(acc, index, &res, 1);
		return res;
	};

// 读取单个三维向量（glm::vec3）
auto ReadVec3 = [](cgltf_accessor* acc, cgltf_size index)
	{
		glm::vec3 res;
		cgltf_accessor_read_float(acc, index, glm::value_ptr(res), 3);
		return res;
	};

// 读取单个四元数（glm::quat），注意顺序转换：glTF(x,y,z,w) → GLM(w,x,y,z)
auto ReadQuat = [](cgltf_accessor* acc, cgltf_size index)
	{
		float res[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		cgltf_accessor_read_float(acc, index, res, 4);
		return glm::quat(res[3], res[0], res[1], res[2]);
	};

// 批量读取所有时间戳（标量数组）
auto ReadTimes = [](cgltf_accessor* acc, std::vector<float>& outTimes)
	{
		outTimes.resize(acc->count);
		for (cgltf_size i = 0; i < acc->count; ++i)
		{
			outTimes[i] = ReadScalar(acc, i);
		}
	};

// 批量读取所有 vec3 关键帧（位置/缩放数组）
auto ReadOutputVec3 = [](cgltf_accessor* acc, std::vector<glm::vec3>& outValues)
	{
		outValues.resize(acc->count);
		for (cgltf_size i = 0; i < acc->count; ++i)
		{
			outValues[i] = ReadVec3(acc, i);
		}
	};

// 批量读取所有 quat 关键帧（旋转数组）——补全的函数
auto ReadOutputQuat = [](cgltf_accessor* acc, std::vector<glm::quat>& outValues)
	{
		outValues.resize(acc->count);
		for (cgltf_size i = 0; i < acc->count; ++i)
		{
			outValues[i] = ReadQuat(acc, i);
		}
	};
void ParseGLTFNode(cgltf_node* node, engine::GameObject* parent, const std::filesystem::path& path) {
	engine::GameObject* obj;
	
	if (!node->name)
	{
		spdlog::info("node has no name");
		obj = parent->GetScene()->CreateGameObject(std::string(""), parent);
	}
	else obj = parent->GetScene()->CreateGameObject(node->name, parent);
	
	if (node->has_matrix) {
		spdlog::info("node has matrix");
		auto mat = glm::make_mat4(node->matrix);

		glm::vec3 translation, scale, skew;
		glm::vec4 perspeective;
		glm::quat orentation;

		glm::decompose(mat, scale, orentation, translation, skew, perspeective);

		obj->SetPosition(translation);
		obj->SetRotation(orentation);
		obj->SetScale(scale);
	}
	else {
		spdlog::warn("node has no matrix");
		if (node->has_translation) {obj->SetPosition(glm::vec3(node->translation[0], node->translation[1], node->translation[2]));}
		if (node->has_rotation) {obj->SetRotation(glm::quat(node->rotation[3],node->rotation[0], node->rotation[1], node->rotation[2]));}
		if (node->has_scale) { obj->SetScale(glm::vec3(node->scale[0], node->scale[1], node->scale[2])); }
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

	if (node->mesh) {
		spdlog::warn("node has no mesh");
		std::shared_ptr<engine::Mesh> mesh = nullptr;
		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		for (cgltf_size i = 0; i < node->mesh->primitives_count; i++) {

			auto& primitive = node->mesh->primitives[i];
			if (primitive.type != cgltf_primitive_type_triangles)
			{
				spdlog::warn("Mesh primitive type is not triangles, skipping");
				continue;
			}

			engine::VertexLayout vertexLayout;
			cgltf_accessor* accessors[4] = { nullptr, nullptr, nullptr,nullptr };

			for (cgltf_size k = 0; k < primitive.attributes_count; ++k)
			{
				cgltf_attribute& attribute = primitive.attributes[k];
				auto acc = attribute.data;
				if (!acc)
				{
					spdlog::warn("Mesh attribute has no accessor, skipping");
					continue;
				}

				engine::VertexElemnet element;
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
				case cgltf_attribute_type_normal:
				{
					if (attribute.index != 0)
					{
						spdlog::warn("Mesh color attribute has index {}, only index 0 is supported, skipping", attribute.index);
						continue;
					}
					accessors[engine::VertexElemnet::Normal] = acc;
					element.attributeLocation = engine::VertexElemnet::Normal;
					element.count = 3;
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

			}
			if (!accessors[engine::VertexElemnet::PositionAttributeLocation])
			{
				spdlog::warn("Mesh primitive has no position attribute, skipping");
				continue;
			}
			auto vertexCount = accessors[engine::VertexElemnet::PositionAttributeLocation]->count;
			vertices.resize(vertexCount * vertexLayout.stride / sizeof(float));
			for (size_t v = 0; v < vertexCount; ++v)
			{
				float* vertexPtr = vertices.data() + v * vertexLayout.stride / sizeof(float);
				for (auto& element : vertexLayout.elements) {
					if (accessors[element.attributeLocation]) {
						readFloats(accessors[element.attributeLocation], v, vertexPtr + element.offset / sizeof(float), element.count);
					}
				}
			}
			if (primitive.indices)
			{
				auto indexCount = primitive.indices->count;
				indices.resize(indexCount);
				for (size_t idx = 0; idx < indexCount; ++idx)
				{
					indices[idx] = readIndex(primitive.indices, idx);
				}
				mesh = std::make_shared<engine::Mesh>(vertexLayout, vertices, indices);
			}
			else {
				mesh = std::make_shared<engine::Mesh>(vertexLayout, vertices);
			}

			auto materialPtr = std::make_shared<engine::Material>();
			materialPtr->SetShaderProgram(engine::Engine::GetInstance().GetGraphicAPI().GetDefaultShaderProgram());

			if (primitive.material) {
				auto material = primitive.material;
				if (material->has_pbr_metallic_roughness) {
					spdlog::info("material has pbr_metallic_roughness");
					auto pbr = material->pbr_metallic_roughness;
					auto texture = pbr.base_color_texture.texture;
					if (texture && texture->image) {
						spdlog::info("use texture image "+std::string(texture->image->uri));
						auto imagePaht = path / std::string(texture->image->uri);
						auto tex = engine::Engine::GetInstance().GetTextureManager().GetTexture(imagePaht.string());
						materialPtr->SetParam("baseColorTexture", tex);
					}
				}
				else if (material->has_pbr_specular_glossiness) {
					spdlog::info("material has pbr_specular_glossiness");
					auto pbr = material->pbr_specular_glossiness;
					auto texture = pbr.diffuse_texture.texture;
					if (texture && texture->image) {
						spdlog::info("use texture image " + std::string(texture->image->uri));
						auto imagePaht = path / std::string(texture->image->uri);
						auto tex = engine::Engine::GetInstance().GetTextureManager().GetTexture(imagePaht.string());
						materialPtr->SetParam("baseColorTexture", tex);
					}
				}

				obj->AddComponent(new engine::MeshComponent(materialPtr, mesh));
			}
		}	
	}
	for (cgltf_size i = 0; i < node->children_count; i++) {
		ParseGLTFNode(node->children[i], obj, path);
	}
}
engine::GameObject* engine::GameObject::LoadGLTF(const std::string& path)
{
	auto contents = engine::Engine::GetInstance().GetFileSystem().LoadAssetsFileText(path);
	if (contents.empty())
	{
		spdlog::error("Failed to load mesh file: {}", path);
		return nullptr;
	}

	cgltf_options options = {};
	cgltf_data* data = nullptr;
	cgltf_result result = cgltf_parse(&options, contents.data(), contents.size(), &data);
	if (result != cgltf_result_success)
	{
		spdlog::error("Failed to parse mesh file: {}, error code: {}", path, static_cast<int>(result));
		return nullptr;
	}
	auto fullPath = engine::Engine::GetInstance().GetFileSystem().GetAssetsPath() / path;
	auto fullFolderPath = fullPath.remove_filename();
	auto relativeFolderPath = std::filesystem::path(path).remove_filename();
	result = cgltf_load_buffers(&options, data, fullFolderPath.string().c_str());

	
	auto obj = engine::Engine::GetInstance().GetCurrentScene()->CreateGameObject("Result");

	auto scene = data->scenes[0];

	for (cgltf_size i = 0; i < scene.nodes_count; i++) {
		auto node = scene.nodes[i];
		ParseGLTFNode(node, obj, relativeFolderPath);
		
	}
	std::vector<std::shared_ptr<engine::AnimationClip>> clips;
	for (cgltf_size ai = 0; ai < data->animations_count; ai++) {
		auto& anim = data->animations[ai];
		auto clip = std::make_shared<engine::AnimationClip>();
		clip->name = anim.name ? anim.name : "animation"+std::to_string(ai);
		clip->duration = 0.0f;

		std::unordered_map<cgltf_node*, size_t> trackIndexOf;
		auto GetOrCreateTrack = [&](cgltf_node* node)->engine::TransformTrack& {
			auto it = trackIndexOf.find(node);
			if (it != trackIndexOf.end()) {
				return clip->tracks[it->second];
			}
			engine::TransformTrack track;
			track.name = node->name ? node->name : "no nmae";
			clip->tracks.push_back(track);
			size_t idx = clip->tracks.size() - 1;
			trackIndexOf[node] = idx;
			return clip->tracks[idx];
			};
		for (cgltf_size ci = 0; ci < anim.channels_count; ci++) {
			auto& channel = anim.channels[ci];
			auto sampler = channel.sampler;

			if (!channel.target_node || !sampler || !sampler->input || !sampler->output) {
				continue;
			}
			std::vector<float> times;
			ReadTimes(sampler->input, times);
			auto& track = GetOrCreateTrack(channel.target_node);

			switch (channel.target_path)
			{
			case cgltf_animation_path_type_translation:
			{
				std::vector<glm::vec3> values;
				ReadOutputVec3(sampler->output, values);
				track.positions.resize(times.size());
				for (cgltf_size i = 0; i < times.size(); i++) {
					track.positions[i].time = times[i];
					track.positions[i].value = values[i];
				}
			}
			break;
			case cgltf_animation_path_type_rotation:
			{
				std::vector<glm::quat> values;
				ReadOutputQuat(sampler->output, values);
				track.rotations.resize(times.size());
				for (cgltf_size i = 0; i < times.size(); i++) {
					track.rotations[i].time = times[i];
					track.rotations[i].value = values[i];
				}
			}
			break;
			case cgltf_animation_path_type_scale:
			{
				std::vector<glm::vec3> values;
				ReadOutputVec3(sampler->output, values);
				track.scales.resize(times.size());
				for (cgltf_size i = 0; i < times.size(); i++) {
					track.scales[i].time = times[i];
					track.scales[i].value = values[i];
				}
			}
			break;
			default:
				break;
			}
			clip->duration = glm::max(clip->duration, times.back());
		}
		clips.push_back(std::move(clip));
	}
	if (!clips.empty()) {
		auto animCom = new AnimationComponet();
		obj->AddComponent(animCom);
		for (auto& clip : clips) {
			animCom->RegisterClip(clip->name , clip);
		}
	}
	cgltf_free(data);
	return obj;
}

engine::GameObject* engine::GameObject::GetChild(const std::string& name)
{
	if (m_name == name) {
		return this;
	}
	else {
		for (auto& child : m_children) {
			auto result =child->GetChild(name);
			if (result != nullptr)
				return result;
		}
	}

	return nullptr;
}

engine::Scene* engine::GameObject::GetScene()
{
	if (!m_scene) {
		spdlog::warn("GameObject {}GetScene return nullptr", m_name);
	}
	return m_scene;
}

bool engine::GameObject::SetParent(GameObject* parent)
{
	if (!m_scene) {
		spdlog::error("GameObject {} m_scene is nullptr, SetParent {} failed", m_name, parent->m_name);
		return false;
	}
	m_scene->SetParent(this, parent);
	return true;
}

void engine::GameObject::MarkForDestory()
{
	m_isAlive = false;
}

void engine::GameObject::SetActive(bool flag)
{
	m_isActive = flag;
}



engine::GameObject* engine::GameObject::GetParent()
{
    return m_parent;
}

glm::vec3 engine::GameObject::GetLocalPosition() const
{
    return m_position;
}

glm::vec3 engine::GameObject::GetWorldPosition() const
{
    glm::vec4 hom = GetWorldTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return glm::vec3(hom) / hom.w;
}

glm::quat engine::GameObject::GetRotation() const
{
    return m_rotation;
}

glm::vec3 engine::GameObject::GetScale() const
{
    return m_scale;
}
