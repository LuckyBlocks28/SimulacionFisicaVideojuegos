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

	rotatePoint(newDirection, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, Vector3D<float>(0.0f, 0.0f, 0.0f));
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
    rotatePoint(finalPos, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, m_position);

    return finalPos;
}

bool SphereGeometry::inArea(Particle* p)
{
    if (!p || !p->isActive()) return false;

    Vector3D<float> diff = p->getPos() - m_position;
    return diff.magnitud() <= m_lenght;
}
