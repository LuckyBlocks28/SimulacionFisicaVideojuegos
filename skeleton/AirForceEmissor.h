#pragma once
#include "ForceEmmiter.h"

class AirForceEmissor : public ForceEmmiter
{
public:
	AirForceEmissor(
		Vector3D<float> pos,
		Vector3D<float> dir,
		Geometry* g,
		Vector3D<float> windVelocity,
		float k1 = 1.0f,
		float k2 = 0.0f,
		bool active = true);

	virtual ~AirForceEmissor() = default;

	// Cambiar a applyForce o addForce según la firma de tu ForceEmmiter.h
	virtual void addForce(Particle* p) override;

	// Método virtual que será sobreescrito por TyphoonForceEmissor
	virtual Vector3D<float> getWindVelocityAt(const Vector3D<float>& particlePos) const;

	inline void setWindVelocity(const Vector3D<float>& windVel) noexcept { m_windVelocity = windVel; }
	inline Vector3D<float> getWindVelocity() const noexcept { return m_windVelocity; }

	inline void setK1(float k1) noexcept { m_k1 = k1; }
	inline void setK2(float k2) noexcept { m_k2 = k2; }

protected:
	Vector3D<float> m_windVelocity;
	float m_k1;
	float m_k2;
};