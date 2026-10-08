#include "Proyectile.h"

Proyectile::Proyectile(Vector3D<float> p, Vector3D<float> v, float size, float mass,Vector4 color):Particle(p,v,size,color),real_Mass(mass), velR(v)
{
	acc = Vector3D<float>(0, -9.8f, 0) * Vector3D<float>(real_Mass, real_Mass, real_Mass);
	vel = velR / velSFactor;

	calculateSimulatedMass();
	calculateSimulateGravity();
}

float Proyectile::calculateSimulatedMass()
{
	return real_Mass * pow((vel.magnitud()/simulated_Velocity), 2);
}

float Proyectile::calculateSimulateGravity()
{
	return real_Gravity * pow((vel.magnitud() / simulated_Velocity), 2);
}

void Proyectile::setMass(float mass) {
	real_Mass = mass;
	calculateSimulatedMass();
}
