#pragma once
#include "AirForceEmissor.h"

class TyphoonForceEmissor : public AirForceEmissor
{
public:
	TyphoonForceEmissor(
		Vector3D<float> pos,
		Vector3D<float> dir,
		Geometry* g,
		float K = 1.0f,
		float k1 = 1.0f,
		float k2 = 0.0f,
		bool active = true);

	virtual ~TyphoonForceEmissor() = default;

	virtual Vector3D<float> getWindVelocityAt(const Vector3D<float>& particlePos) const override;

	inline void setK(float K) noexcept { m_K = K; }
	inline float getK() const noexcept { return m_K; }

private:
	float m_K;
};