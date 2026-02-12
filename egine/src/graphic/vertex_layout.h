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

		static constexpr int PositionAttributeLocation = 0;
		static constexpr int ColorAttributeLocation = 1;
		static constexpr int UVAttributeLocation = 2;
	};

	struct VertexLayout {
		std::vector<VertexElemnet> elements;
		GLsizei stride = 0;
		
		VertexLayout() = default;
		VertexLayout(const std::vector<VertexElemnet>& elements,GLsizei stride);

		void logInfo();
	};
}