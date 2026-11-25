#include "AnchoredSpringForceEmissor.h"

AnchoredSpringForceEmissor::AnchoredSpringForceEmissor(double k, double restLength, Particle* particle,
	physx::PxTransform* anchorPose, Vector3D<float> localOffset,
	int numSpheres, float sphereRadius, Vector4 color)
	: SpringForceEmissor(k, restLength, particle, nullptr, numSpheres, sphereRadius, color),
	_anchorPose(anchorPose), _localOffset(localOffset)
{
	updateVisualTransform();
}

bool AnchoredSpringForceEmissor::isEndpointBValid() const
{
	return _anchorPose != nullptr;
}

Vector3D<float> AnchoredSpringForceEmissor::getEndpointBPos() const
{
	physx::PxVec3 worldAnchorPos = _anchorPose->transform(_localOffset);
	return Vector3D<float>(worldAnchorPos.x, worldAnchorPos.y, worldAnchorPos.z);
}