#include "vertex_layout.h"
#include "spdlog/spdlog.h"

engine::VertexLayout::VertexLayout(const std::vector<engine::VertexElemnet>& elements,GLsizei stride):elements(elements),stride(stride)
{
	
}

void engine::VertexLayout::logInfo()
{
	for (auto& element : elements) {
		spdlog::info("vertexLayout ::{},{},{},{}", element.attributeLocation, element.count, element.dataType, element.offset);
	}
	spdlog::info("{}", stride);
}
