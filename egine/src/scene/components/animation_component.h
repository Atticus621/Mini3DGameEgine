#pragma once

#include "scene/component.h"

#include <glm/gtx/quaternion.hpp>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include <memory>

namespace engine {
	struct KeyFragmentVec3 {
		float time = 0.0;
		glm::vec3 value{ 0.0f };
	};
	struct KeyFragmentQuat {
		float time = 0.0;
		glm::quat value{ 1.0f,0.0f,0.0f,0.0f };
	};

	struct TransformTrack {
		std::string name;
		std::vector<KeyFragmentVec3> positions;
		std::vector<KeyFragmentQuat> rotations;
		std::vector<KeyFragmentVec3> scales;
	};
	struct AnimationClip {
		std::string name;
		float duration = 0.0;
		bool loop = true;
		std::vector<TransformTrack> tracks;
	};
	struct ObjectBind {
		GameObject* obj{ nullptr };
		std::vector<size_t> indices;

	};
	class AnimationComponet :public Component {
		COMPONENT(AnimationComponet);

	public:
		void Update(float delta)override;
		void SetClip(AnimationClip*);
		void RegisterClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip);
		bool Play(const std::string& name, bool loop = true);

	private:
		void BuildBinds();
		glm::vec3 Interpolate(const std::vector<KeyFragmentVec3>& keys, float time);
		glm::quat Interpolate(const std::vector<KeyFragmentQuat>& keys, float time);
	private:
		AnimationClip* m_clip{ nullptr };
		float m_time = 0.0;
		bool m_loop = true;
		bool m_play = false;

	private:
		std::unordered_map<std::string, std::shared_ptr<AnimationClip>> m_clips;
		std::unordered_map<GameObject*, std::shared_ptr<ObjectBind>> m_binds;
	};
}