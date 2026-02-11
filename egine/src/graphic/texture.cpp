#include "texture.h"
#include <spdlog/spdlog.h>
#include "engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
engine::Texture::Texture(int width, int height, int channels, unsigned char* data)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		spdlog::error("OpenGL error after setting material texture: 0x{:X}", error);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

engine::Texture::~Texture()
{
	if (m_textureID > 0) {
		glDeleteTextures(1, &m_textureID);
		spdlog::info("Texture with ID {} deleted", m_textureID);
	}
}

bool engine::Texture::Init(int width, int height, int channels, unsigned char* data)
{
	return false;
}

GLuint engine::Texture::GetTextureID() const
{
    if (m_textureID == 0)
    {
		spdlog::warn("Texture ID is invalid: {}", m_textureID);
    }
    return m_textureID;
}

std::shared_ptr<engine::Texture> engine::Texture::load(const std::string& filePath)
{
	auto& fs = engine::Engine::GetInstance().GetFileSystem();
	auto path = fs.GetAssetsPath() / filePath;

	if (!std::filesystem::exists(path)) {
		spdlog::error("Texture file does not exist: {}", path.string());
		return nullptr;
	}

	int width = 0, height = 0, channels = 3;
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
	std::shared_ptr<engine::Texture> texture;
	if (data) {

		texture = std::make_shared<engine::Texture>(width, height, channels, data);

		spdlog::info("Image loaded successfully: {} ({}x{}, {} channels)", path.string(), width, height, channels);
		stbi_image_free(data);
		return texture;
	}
	else {
		spdlog::error("Failed to load image: {}", path.string());
		return nullptr;
	}
}
