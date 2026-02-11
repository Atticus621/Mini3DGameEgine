#pragma once
#include "GLES3/gl3.h"

namespace engine
{
	class Texture
	{
	public:

		Texture(int width, int height, int channels,unsigned char* data);
		~Texture();

		GLuint GetTextureID() const;

	private:
		GLuint m_textureID = 0;
		int m_width = 0;
		int m_height = 0;
		int m_channels = 0;
	};
};