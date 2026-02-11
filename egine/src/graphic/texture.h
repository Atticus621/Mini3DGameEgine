#pragma once
#include "GLES3/gl3.h"
#include <memory>
#include <string>

namespace engine
{
	class Texture
	{
	public:

		Texture(int width, int height, int channels,unsigned char* data);
		~Texture();
		bool Init(int width, int height, int channels, unsigned char* data);

		GLuint GetTextureID() const;
		static std::shared_ptr<Texture> load(const std::string& filePath);

	private:
		GLuint m_textureID = 0;
		int m_width = 0;
		int m_height = 0;
		int m_channels = 0;
	};
};