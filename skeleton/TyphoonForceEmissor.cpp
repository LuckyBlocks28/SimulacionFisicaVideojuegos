#include "TyphoonForceEmissor.h"

TyphoonForceEmissor::TyphoonForceEmissor(ForceSystem* fS, Vector3D<float> pos, Vector3D<float> dir, Geometry* g, float K, float k1, float k2, bool active) :
	AirForceEmissor(fS, pos, dir, g, Vector3D<float>(0.0f, 0.0f, 0.0f), k1, k2, active), m_K(K)
{
}

Vector3D<float> TyphoonForceEmissor::getWindVelocityAt(const Vector3D<float>& particlePos) const
{
	// Desplazamiento respecto al centro del torbellino (m_position)
	float dx = particlePos.getX() - m_position.getX();
	float dy = particlePos.getY() - m_position.getY();
	float dz = particlePos.getZ() - m_position.getZ();

	// Campo de velocidades tangencial + corriente vertical
	// v_x = -K * (z - z_c)
	// v_y =  K * (50 - (y - y_c))
	// v_z =  K * (x - x_c)
	Vector3D<float> whirlwindVel(
		-dz,
		50.0f - dy,
		dx
	);

	return whirlwindVel * m_K;
}
