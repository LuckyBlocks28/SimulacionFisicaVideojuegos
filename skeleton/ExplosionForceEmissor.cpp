#include "ExplosionForceEmissor.h"

ExplosionForceEmissor::ExplosionForceEmissor(Vector3D<float> pos, Geometry* g, float K, float tau, float radius, bool active)
	: ForceEmmiter(pos, Vector3D<float>(0.0f, 1.0f, 0.0f), g, 0.0f, active),
	m_K(K), m_tau(tau), m_radius(radius), m_time(0.0f)
{
}

void ExplosionForceEmissor::explode()
{
	m_time = 0.0f;
	m_active = true;
}

void ExplosionForceEmissor::explode(const Vector3D<float>& customPos)
{
	m_position = customPos;
	m_time = 0.0f;
	m_active = true;
}

void ExplosionForceEmissor::update(double dt)
{
	if (!m_active) return;

	m_time += dt;

	// Transcurridos 4 * tau, la fuerza es prácticamente 0 (e^-4 ? 0.018) y se desactiva
	if (m_time >= 4.0f * m_tau) {
		m_active = false;
	}
}

void ExplosionForceEmissor::addForce(Particle* p)
{
	if (!m_active || !p || !p->isActive()) return;

	// Vector diferencia desde el centro de la explosión a la partícula
	Vector3D<float> diff = p->getPos() - m_position;
	float r = diff.magnitud();

	// Comprobar si la partícula está dentro del radio R
	if (r > 0.0001f && r < m_radius)
	{
		// Amortiguación temporal exponencial: e^(-t / tau)
		float timeDecay = std::exp(-m_time / m_tau);

		// Magnitud y dirección: (K / r^2) * (p - p_c) * e^(-t/tau)
		float factor = (m_K / (r * r)) * timeDecay;

		Vector3D<float> explosionForce = diff * factor;

		// Inyectar fuerza a la partícula
		p->addForce(explosionForce);
	}
}