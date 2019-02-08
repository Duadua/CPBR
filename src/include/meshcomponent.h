#pragma once

#include "scenecomponent.h"

class Mesh;

class MeshComponent : public SceneComponent {
	DECLARE_CLASS(MeshComponent)
public:
	MeshComponent(Mesh* m = nullptr);
	virtual ~MeshComponent();

	virtual void draw(SPTR_Shader shader) override;
	void set_mesh(Mesh* m);

private:
	Mesh* mesh;
};
DECLARE_AUTO_PTR(MeshComponent)