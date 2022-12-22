#include "UnitCube.h"
#include <glm/gtx/transform.hpp>

void colouredTriangles(CPU_Geometry& geom, glm::vec3 col);
void positiveZFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom);
void positiveXFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom);
void negativeZFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom);
void negativeXFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom);
void positiveYFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom);
void negativeYFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom);

void UnitCube::generateGeometry() {
	std::vector<glm::vec3> originQuad;
	originQuad.push_back(glm::vec3{ -0.5, 0.5, 0 }); // top-left
	originQuad.push_back(glm::vec3{ -0.5, -0.5, 0 }); // bottom-left
	originQuad.push_back(glm::vec3{ 0.5, 0.5, 0 }); // top-right

	originQuad.push_back(glm::vec3{ -0.5, -0.5, 0 }); // bottom-left
	originQuad.push_back(glm::vec3{ 0.5, -0.5, 0 }); // bottom-right
	originQuad.push_back(glm::vec3{ 0.5, 0.5, 0 }); // top-right

	CPU_Geometry square;



	positiveZFace(originQuad, square);
	colouredTriangles(square, { 1.f, 1.f, 0.f });

	positiveXFace(originQuad, square);
	colouredTriangles(square, { 1.f, 0.f, 0.f });

	negativeZFace(originQuad, square);
	colouredTriangles(square, { 0.f, 1.f, 0.f });

	negativeXFace(originQuad, square);
	colouredTriangles(square, { 0.f, 0.f, 1.f });

	positiveYFace(originQuad, square);
	colouredTriangles(square, { 1.f, 0.f, 1.f });

	negativeYFace(originQuad, square);
	colouredTriangles(square, { 0.f, 1.f, 1.f });

	m_gpu_geom.bind();
	m_gpu_geom.setVerts(square.verts);
	m_gpu_geom.setCols(square.cols);
	m_gpu_geom.setNormals(square.normals);

	m_size = square.verts.size();
}

void colouredTriangles(CPU_Geometry& geom, glm::vec3 col) {
	geom.cols.push_back(col);
	geom.cols.push_back(col);
	geom.cols.push_back(col);
	geom.cols.push_back(col);
	geom.cols.push_back(col);
	geom.cols.push_back(col);
}

void positiveZFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom) {
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.5));
	for (auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, 1.0));
}

void positiveXFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	for (auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(1.0, 0.0, 0.0));
}

void negativeZFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
	for (auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
	geom.normals.push_back(glm::vec3(0.0, 0.0, -1.0));
}

void negativeXFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
	for (auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
	geom.normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
}

void positiveYFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	for (auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, 1.0, 0.0));
}

void negativeYFace(std::vector<glm::vec3> const& originQuad, CPU_Geometry& geom) {
	const glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
	for (auto i = originQuad.begin(); i < originQuad.end(); ++i) {
		geom.verts.push_back(
			glm::vec3(T * R * glm::vec4((*i), 1.0))
		);
	}
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
	geom.normals.push_back(glm::vec3(0.0, -1.0, 0.0));
}
