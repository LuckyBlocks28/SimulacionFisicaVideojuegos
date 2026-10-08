#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D<float> p, Vector3D<float> v,float size, Vector4 color = Vector4(1.0f,0.0f,0.0f,1.0f));
	virtual ~Particle();
	void integrateEuler(float t);
	void integrateEulerSemi(float t);
	void integrateVerlet(float t);
	void setOriginalPosition();
	void setDamping(float d);

protected:
	Vector3D<float> vel;
	Vector3D<float> acc = { 0.0f, 9.8f, 0.0f };
	float damping;

	physx::PxTransform pose;
	physx::PxTransform antPos;
	RenderItem* renderItem;

	Vector3D<float> ogPos;
	bool firstMove;

};

