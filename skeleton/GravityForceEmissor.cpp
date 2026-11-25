#include "GravityForceEmissor.h"
#include <iostream>

GravityForceEmissor::GravityForceEmissor(Vector3D<float> pos, Vector3D<float> dir, Geometry* g, float gravityMagnitude, bool active, GravityType type)
	: ForceEmmiter(pos, dir, g, gravityMagnitude, active), m_type(type)
{
}
void GravityForceEmissor::addForce(Particle* p)
{
	if (!m_active || !m_geom || !p || !p->isActive()) return;

	// Verificar si la partícula está dentro del área de la geometría
	if (m_geom->inArea(p))
	{
		Vector3D<float> forceVector(0.0f, 0.0f, 0.0f);

		if (m_type == GravityType::Directional)
		{
			// F = m * g * dirección
			Vector3D<float> dirNorm = m_direction;
			if (dirNorm.magnitud() > 0.0001f) {
				dirNorm.normalize();
			}
			else {
				dirNorm = Vector3D<float>(0.0f, -1.0f, 0.0f); // Dirección por defecto hacia abajo
			}

			forceVector = dirNorm * (m_force * p->getMass());
		}
		else if (m_type == GravityType::Point)
		{
			Vector3D<float> dirToCenter = m_position - p->getPos();
			float dist = dirToCenter.magnitud();

			if (dist > 0.0001f) {
				dirToCenter.normalize();
				// Fuerza proporcional a la masa
				forceVector = dirToCenter * (m_force * p->getMass());
			}
		}

		// Acumular la fuerza en la partícula
		p->addForce(forceVector);
	}
}