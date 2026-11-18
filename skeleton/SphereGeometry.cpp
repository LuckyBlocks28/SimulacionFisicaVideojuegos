#include "SphereGeometry.h"

SphereGeometry::SphereGeometry(Vector3D<float> pos, Vector3D<float> dir, float l):Geometry(pos,dir),m_lenght(l),r(0.0f),theta(0.0f)
{
}

Vector3D<float> SphereGeometry::calculateDirection(float factor)
{
	Vector3D<float> newDirection = offset;
	if (newDirection.magnitud() < 0.0001f) {
		newDirection = Vector3D<float>(0.0f, 1.0f, 0.0f);
	}

	Vector3D<float> up(0.0f, 1.0f, 0.0f);
	if ((up - m_direction).magnitud() > 0.0001f) {
		rotatePoint(newDirection, up, m_direction, Vector3D<float>(0.0f, 0.0f, 0.0f));
	}

	newDirection.normalize();
	return newDirection;
}

Vector3D<float> SphereGeometry::calculateNewPosition(float factor)
{
	float normFactor = std::clamp(factor, 0.0f, 1.0f);
	float u1 = normFactor;
	float u2 = std::fmod(normFactor * 13.3714f + 0.31415f, 1.0f);
	float u3 = std::fmod(normFactor * 47.1132f + 0.71828f, 1.0f);

	r = m_lenght * std::cbrt(u1);
	float cosTheta = 1.0f - 2.0f * u2;
	float sinTheta = std::sqrt(max(0.0f, 1.0f - cosTheta * cosTheta));
	float phi = u3 * TWO_PI;

	x = r * sinTheta * std::cos(phi);
	y = r * sinTheta * std::sin(phi);
	z = r * cosTheta;
	offset = Vector3D<float>(x, y, z);

	Vector3D<float> finalPos = m_position + offset;
	Vector3D<float> up(0.0f, 1.0f, 0.0f);
	if ((up - m_direction).magnitud() > 0.0001f) {
		rotatePoint(finalPos, up, m_direction, m_position);
	}

	return finalPos;
}

void SphereGeometry::containParticle(Particle* p, float restitution)
{
	if (!p || !p->isActive()) return;

	Vector3D<float> diff = p->getPos() - m_position;
	float dist = diff.magnitud();

	// Si la partícula intenta salir del radio m_lenght
	if (dist > m_lenght && dist > 0.0001f) {
		// 1. Normal unitaria hacia el exterior en el punto de impacto
		Vector3D<float> normal = diff / dist;

		// 2. Recolocar la partícula exactamente en la pared interior de la esfera
		Vector3D<float> clampedPos = m_position + normal * m_lenght;
		p->setPosition(clampedPos);

		// 3. Reflejar la velocidad solo si se mueve hacia afuera (producto escalar > 0)
		Vector3D<float> vel = p->getVel();
		float vDotN = vel.getX() * normal.getX() +
			vel.getY() * normal.getY() +
			vel.getZ() * normal.getZ();

		if (vDotN > 0.0f) {
			Vector3D<float> reflectedVel = vel - normal * ((1.0f + restitution) * vDotN);
			p->setVelocity(reflectedVel);
		}
	}
}

bool SphereGeometry::inArea(Particle* p)
{
    if (!p || !p->isActive()) return false;

    Vector3D<float> diff = p->getPos() - m_position;
    return diff.magnitud() <= m_lenght;
}
