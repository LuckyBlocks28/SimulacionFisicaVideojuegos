#include "AirForceEmissor.h"

AirForceEmissor::AirForceEmissor(ForceSystem* fS, Vector3D<float> pos, Vector3D<float> dir, Geometry* g, Vector3D<float> windVelocity, float k1, float k2, bool active)
	: ForceEmmiter(fS, pos, dir, g, 0.0f, active), m_windVelocity(windVelocity), m_k1(k1), m_k2(k2)
{
}

Vector3D<float> AirForceEmissor::getWindVelocityAt(const Vector3D<float>& particlePos) const
{
	return m_windVelocity; // En el emisor estándar el viento es constante
}

void AirForceEmissor::addForce(Particle* p, double dt)
{
	if (!m_active || !m_geom || !p || !p->isActive()) return;

	if (m_geom->inArea(p))
	{
		// LLAMADA POLIMÓRFICA: Obtener el viento según la posición (Torbellino u otro campo)
		Vector3D<float> currentWindVel = getWindVelocityAt(p->getPos());

		// Velocidad relativa
		Vector3D<float> diffVel = currentWindVel - p->getVel();
		float diffMag = diffVel.magnitud();

		// Términos de rozamiento aerodinámico
		Vector3D<float> fLinear = diffVel * m_k1;
		Vector3D<float> fQuadratic = diffVel * (m_k2 * diffMag);

		Vector3D<float> totalWindForce = fLinear + fQuadratic;
		p->addForce(totalWindForce);
	}
}