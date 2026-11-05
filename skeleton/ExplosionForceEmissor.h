#pragma once
#include "ForceEmmiter.h"
#include <cmath>

class ExplosionForceEmissor : public ForceEmmiter
{
public:
	ExplosionForceEmissor(ForceSystem* fS,Vector3D<float> pos,Geometry* g,float K = 5000.0f,float tau = 0.5f,float radius = 30.0f,bool active = false);

	virtual ~ExplosionForceEmissor() = default;

	// Actualiza el tiempo transcurrido t de la explosión
	virtual void update(double dt) override;

	// Aplica la fuerza radial exponencialmente amortiguada
	virtual void addForce(Particle* p, double dt) override;

	// Detona la explosión en la posición actual o en una nueva coordenada
	void explode();
	void explode(const Vector3D<float>& customPos);

	inline bool isFinished() const noexcept { return m_time >= 4.0f * m_tau; }
	inline float getTime() const noexcept { return m_time; }

private:
	float m_K;      // Intensidad de la explosión
	float m_tau;    // Constante de tiempo
	float m_radius; // Radio máximo de acción (R)
	float m_time;   // Tiempo transcurrido (t)
};