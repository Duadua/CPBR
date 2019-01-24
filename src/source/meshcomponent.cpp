#include "mesh.h"
#include "meshcomponent.h"

MeshComponent::MeshComponent(Mesh* m) {
	mesh = m;
}

MeshComponent::~MeshComponent() {
	delete mesh;
}

void MeshComponent::set_mesh(Mesh* m) {
	mesh = m;
}

void MeshComponent::draw(Shader& shader) {
	if (mesh != nullptr) {
		shader.set_mat4("model", get_transform());
		qDebug() << get_transform() << endl;
		mesh->draw();
	}
	for (auto cc : child_components) {
		cc->draw(shader);
	}

}
