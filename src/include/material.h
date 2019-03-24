#pragma once

#include "cmath.h"
#include "reflex.h"
#include <string>

DECLARE_AUTO_PTR(Material)
class Material : public CObject {
	DECLARE_CLASS(Material)
public:
	Material();
	Material(const Material& b);
	void copy_from(const SPTR_Material b);
	~Material() {}

	void use(const std::string& shader);
	static void un_use(const std::string& shader);

    GET_SET(std::string, name)

    GET_SET(CVector3D, ka)
    GET_SET(CVector3D, kd)
    GET_SET(CVector3D, ks)

    GET_SET(std::string, map_ka)
    GET_SET(std::string, map_kd)
    GET_SET(std::string, map_ks)

    GET_SET(float, shininess)

	static std::string default_material_name;

private:
	std::string name;

	CVector3D ka;
	CVector3D kd;
	CVector3D ks;

	float shininess;

	std::string map_ka;
	std::string map_kd;
	std::string map_ks;

};
