#pragma once

#include <GLES3/gl3.h>
#include <stdint.h>
#include <vector>


namespace engine {

	struct VertexElemnet
	{
		GLuint attributeLocation;
		GLint count;
		GLenum dataType;
		uint32_t offset;
	};

	struct VertexLayout {
		std::vector<VertexElemnet> elements;
		GLsizei stride;
	};
}