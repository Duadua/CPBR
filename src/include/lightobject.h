#pragma once

#include "gameobject.h"

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)
PRE_DECLARE_CLASS(LightComponent)
USING_SPTR(LightComponent)
PRE_DECLARE_CLASS(DirectLightComponent)
USING_SPTR(DirectLightComponent)
PRE_DECLARE_CLASS(PointLightComponent)
USING_SPTR(PointLightComponent)
PRE_DECLARE_CLASS(SpotLightComponent)
USING_SPTR(SpotLightComponent)
PRE_DECLARE_CLASS(SkyLightComponent)
USING_SPTR(SkyLightComponent)

enum LightType {
	DIRECT_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
	SKY_LIGHT
};

class LightObject : public GameObject {
public:
	LightObject() {}
	virtual ~LightObject() {}

	LightType get_light_type() { return light_type; }

    bool use(SPTR_Shader shader);

protected:

	LightType light_type;

};
DECLARE_AUTO_PTR(LightObject)


// ===========================================================================

PRE_DECLARE_CLASS(MeshComponent)
USING_SPTR(MeshComponent)

class DirectLightObject : public LightObject {
	DECLARE_CLASS(DirectLightObject)
	
public:

	DirectLightObject();
	virtual ~DirectLightObject() {}

	SPTR_DirectLightComponent get_light_component() { return std::dynamic_pointer_cast<DirectLightComponent>(root_component); }

	virtual void draw(SPTR_Shader shader) override;
private:
	SPTR_MeshComponent m_mesh_component;
};
DECLARE_AUTO_PTR(DirectLightObject)

// ===========================================================================

class PointLightObject : public LightObject {
	DECLARE_CLASS(PointLightObject)
	
public:

	PointLightObject();
	virtual ~PointLightObject() {}

	SPTR_PointLightComponent get_light_component() { return std::dynamic_pointer_cast<PointLightComponent>(root_component); }
	
	virtual void draw(SPTR_Shader shader) override;
private:
	SPTR_MeshComponent m_mesh_component;

};
DECLARE_AUTO_PTR(PointLightObject)

// ===========================================================================

class SpotLightObject : public LightObject {
	DECLARE_CLASS(SpotLightObject)
	
public:

	SpotLightObject();
	virtual ~SpotLightObject() {}

	SPTR_SpotLightComponent get_light_component() { return std::dynamic_pointer_cast<SpotLightComponent>(root_component); }

private:

};
DECLARE_AUTO_PTR(SpotLightObject)

// ===========================================================================

class SkyLightObject : public LightObject {
	DECLARE_CLASS(SkyLightObject)
	
public:

	SkyLightObject();
	virtual ~SkyLightObject() {}

	SPTR_SkyLightComponent get_light_component() { return std::dynamic_pointer_cast<SkyLightComponent>(root_component); }
private:

};
DECLARE_AUTO_PTR(SkyLightObject)


