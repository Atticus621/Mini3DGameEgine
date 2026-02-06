#include "input_manager.h"
#include <spdlog/spdlog.h>

void engine::InputManager::SetKeyPressed(int key, bool pressed)
{
	if (key<0 || key>=m_keys.size()) {
		spdlog::error("{} is out of bound", key);
	}
	m_keys.at(key) = pressed;
}

bool engine::InputManager::GetKeyPressed(int key)
{
	if (key < 0 || key >= m_keys.size()) {
		spdlog::error("{} is out of bound", key);
	}
	return m_keys.at(key);
}
