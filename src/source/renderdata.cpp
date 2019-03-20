#define GLEW_STATIC
#include <GL/glew.h>

#include "renderdata.h"

#include <GLFW/glfw3.h>

IMPLEMENT_CLASS(RenderData)

RenderData::RenderData() : vao(0), vbo(0), ebo(0) {
	material_name = "default";		// default material
}
RenderData::~RenderData() {
	//if (vao != 0) core->glDeleteVertexArrays(1, &vao);
	//if (vbo != 0) core->glDeleteBuffers(1, &vbo);
	//if (ebo != 0) core->glDeleteBuffers(1, &ebo);

}

void RenderData::init(const std::vector<MVertex>& v, const std::vector<uint>& i) {
	vertices = v;
	indices = i;

	// 初始化 vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(MVertex)), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 初始化 ebo
	if (indices.size() > 0) {
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint)), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// 初始化 vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (indices.size() > 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), reinterpret_cast<void*>(offsetof(MVertex, normal)));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MVertex), reinterpret_cast<void*>(offsetof(MVertex, tex_coord)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (indices.size() > 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderData::draw() {
	glBindVertexArray(vao);
    if(indices.size() <= 0) glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    else glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

// ===========================================================================

RenderDataReprocess* RenderDataReprocess::instance = nullptr;
RenderDataReprocess* RenderDataReprocess::get_instance() {
	if (instance == nullptr) { instance = new RenderDataReprocess(); }
	return instance;
}
RenderDataReprocess& renderdata_reprocess() { return (*RenderDataReprocess::get_instance()); }

void RenderDataReprocess::bump(std::vector<MVertex>& res_v, const std::vector<MVertex>& v, int step, float out_scope, float in_scope) {
	step = CMath::clamp(step, 1, 1000); step *= 3; 
	
	res_v.clear(); res_v.assign(v.begin(), v.end()); int i = 0;
	for (auto& mv : res_v) { if (i%step == 0) mv.position += mv.normal * CMath::random(-in_scope, out_scope); ++i; }
}

void RenderDataReprocess::smooth(std::vector<MVertex>& res_v, std::vector<uint>& res_i,
	const std::vector<MVertex>& v, const std::vector<uint>& i) {

}



