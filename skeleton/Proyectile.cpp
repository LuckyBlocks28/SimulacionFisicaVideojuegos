#include "Proyectile.h"

Proyectile::Proyectile(Vector3D<float> p, Vector3D<float> v, float size, float mass, Vector4 color)
	: Particle(p, v, size, mass, color), velR(v) // Orden correcto de size y mass
{
	// 1. Calcular velocidades primero
	vel = velR / velSFactor;
	simulated_Velocity = vel.magnitud();

	// 2. Calcular masa y gravedad simulada una vez asignada simulated_Velocity
	simulated_Mass = calculateSimulatedMass();
	simulated_Gravity = calculateSimulateGravity();

	acc = Vector3D<float>(0.0f, -std::abs(simulated_Gravity), 0.0f);
	
	damping = 0.999f;
}

float Proyectile::calculateSimulatedMass()
{
	if (simulated_Velocity < 0.0001f) return real_Mass;
	float ratio = vel.magnitud() / simulated_Velocity;
	return real_Mass * (ratio * ratio);
}

float Proyectile::calculateSimulateGravity()
{
	if (simulated_Velocity < 0.0001f) return real_Gravity;
	float ratio = vel.magnitud() / simulated_Velocity;
	return real_Gravity * (ratio * ratio);
}