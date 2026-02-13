#include "animation_component.h"
#include "scene/game_object.h"

#include <spdlog/spdlog.h>

void engine::AnimationComponet::Update(float delta)
{
    if (m_clips.empty())return;
    if (!m_play) return;

    m_time += delta;
    if (m_time > m_clip->duration) {
        if (m_loop) {
            m_time = std::fmod(m_time, m_clip->duration);
        }
        else {
            m_time = 0;
            m_play = false;
            return;
        }
    }

    for (auto& bind : m_binds) {
        auto& obj = bind.first;
        auto& tranckIndices = bind.second->indices;
        for (auto index : tranckIndices) {
            auto& track = m_clip->tracks[index];
            if (!track.positions.empty()) {
                auto pos = Interpolate(track.positions, m_time);
                obj->SetPosition(pos);
            }
            if (!track.rotations.empty()) {
                auto rotation = Interpolate(track.rotations, m_time);
                obj->SetRotation(rotation);
            }
            if (!track.scales.empty()) {
                auto scale = Interpolate(track.scales, m_time);
                obj->SetScale(scale);
            }
        }
    }
}
glm::vec3 engine::AnimationComponet::Interpolate(const std::vector<engine::KeyFragmentVec3>& keys, float time)
{
    if (keys.empty()) {return glm::vec3(0.0f);}
    if (keys.empty()) { return keys[0].value; }
    if (time <= keys.front().time) { return keys.front().value; }
    if (time >= keys.back().time) { return keys.back().value; }

    size_t i0 = 0, i1 = 0;
    for (size_t i = 0; i < keys.size(); ++i) {
        if (time <= keys[i].time) {
            i1 = i;
            break;
        }
    }
    i0 = i1 > 0 ? i1 - 1 : 0;

    if (time >= keys[i0].time && time <= keys[i1].time) {
        float delta = keys[i1].time - keys[i0].time;
        float k = (time - keys[i0].time) / delta;

        return glm::mix(keys[i0].value, keys[i1].value, k);
    }
    return keys.back().value;
}
glm::quat engine::AnimationComponet::Interpolate(const std::vector<engine::KeyFragmentQuat>& keys, float time)
{
    if (keys.empty()) { return glm::quat(1.0f,0.0f,0.0f,0.0f); }
    if (keys.empty()) { return keys[0].value; }
    if (time <= keys.front().time) { return keys.front().value; }
    if (time >= keys.back().time) { return keys.back().value; }

    size_t i0 = 0, i1 = 0;
    for (size_t i = 0; i < keys.size(); ++i) {
        if (time <= keys[i].time) {
            i1 = i;
            break;
        }
    }
    i0 = i1 > 0 ? i1 - 1 : 0;

    if (time >= keys[i0].time && time <= keys[i1].time) {
        float delta = keys[i1].time - keys[i0].time;
        float k = (time - keys[i0].time) / delta;

        return glm::slerp(keys[i0].value, keys[i1].value, k);
    }
    return keys.back().value;
}
void engine::AnimationComponet::SetClip(engine::AnimationClip*)
{
    m_clip = clip;
    BuildBinds();
}

void engine::AnimationComponet::RegisterClip(const std::string& name, const std::shared_ptr<engine::AnimationClip>& clip)
{
    static int temName = 0;
    if (name.empty()) {
        m_clips["Clip"+std::to_string(temName)] = clip;
        temName++;
    }
    m_clips[name] = clip;
}

bool engine::AnimationComponet::Play(const std::string& name, bool loop)
{
    if (m_clip && m_clip->name == name) {
        m_time = 0.0f;
        m_play = true;
        m_loop = loop;
        return true;
    }
    else
    {
        auto it = m_clips.find(name);
        if (it != m_clips.end()) {
            SetClip(it->second.get());
            m_time = 0.0f;
            m_play = true;
            m_loop = loop;
            return true;
        }
        else {
            spdlog::error("clip: {} not exits,registe {} before play", name, name);
            return false;
        }
    }
}

void engine::AnimationComponet::BuildBinds()
{
    m_binds.clear();
    if (!m_clip)return;

    for (size_t i = 0; i < m_clip->tracks.size(); ++i) {
        auto& track = m_clip->tracks[i];
        auto target = m_owner->GetChild(track.name);

        if (target) {
            auto it = m_binds.find(target);
            if (it != m_binds.end()) {
                it->second->indices.push_back(i);
            }
            else {
                auto bind = std::make_shared<engine::ObjectBind>();
                bind->obj = target;
                bind->indices.push_back(i);
                m_binds.emplace(target, std::move(bind));
            }
        }
        else {
            spdlog::critical("{} can find in {}", track.name, m_owner->GetName());
            return;
        }
       
    }
}


