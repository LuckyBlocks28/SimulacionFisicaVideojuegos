#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D<float> p, Vector3D<float> v, Vector3D<float> a,float size, Vector4 color = Vector4(1.0f,0.0f,0.0f,1.0f));
	~Particle();
	void release();
	void integrateEuler(float t);
	void integrateEulerSemi(float t);
	void setOriginalPosition();
	void setDamping(float d);

protected:
	Vector3D<float> vel;
	Vector3D<float> acc;
	float damping;

	physx::PxTransform pose;
	RenderItem* renderItem;

	Vector3D<float> ogPos;
};

