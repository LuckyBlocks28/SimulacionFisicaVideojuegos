#include "Proyectile.h"

Proyectile::Proyectile(Vector3D<float> p, Vector3D<float> v, float size, float mass,Vector4 color):
	Particle(p,v,size,color),real_Mass(mass), velR(v),simulated_Mass(calculateSimulatedMass()),simulated_Gravity(calculateSimulateGravity())
{
	vel = velR / velSFactor;
	simulated_Velocity = vel.magnitud();

	acc = Vector3D<float>(0.0f, simulated_Gravity, 0.0f);
}

float Proyectile::calculateSimulatedMass()
{
	float real_vel_mag = velR.magnitud();
	if (real_vel_mag == 0.0f) return real_Mass;
	return real_Mass * pow((vel.magnitud()/simulated_Velocity), 2);
}

float Proyectile::calculateSimulateGravity()
{
	float real_vel_mag = velR.magnitud();
	if (real_vel_mag == 0.0f) return real_Gravity;

	return real_Gravity * pow((vel.magnitud() / simulated_Velocity), 2);
}

void Proyectile::setMass(float mass) {
	real_Mass = mass;
	calculateSimulatedMass();
}
