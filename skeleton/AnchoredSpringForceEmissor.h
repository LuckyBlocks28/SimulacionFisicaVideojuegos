#pragma once
#include "SpringForceEmissor.h"

class AnchoredSpringForceEmissor : public SpringForceEmissor
{
public:
	// Recibe directamente un puntero al PxTransform del objeto fijo
	AnchoredSpringForceEmissor(double k, double restLength, Particle* particle, physx::PxTransform* anchorPose,
		Vector3D<float> localOffset = Vector3D<float>(0.0f, 0.0f, 0.0f),
		int numSpheres = 8, float sphereRadius = 0.6f,
		Vector4 color = Vector4(0.8f, 0.8f, 0.2f, 1.0f));

	virtual ~AnchoredSpringForceEmissor() = default;

protected:
	physx::PxTransform* _anchorPose = nullptr;
	physx::PxVec3 _localOffset;

	virtual bool isEndpointBValid() const override;
	virtual Vector3D<float> getEndpointBPos() const override;
};