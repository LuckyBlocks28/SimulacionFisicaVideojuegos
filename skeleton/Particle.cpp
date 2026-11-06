#include "Particle.h"



Particle::Particle() : ogPos(0, 0, 0), vel(0, 0, 0), acc(0, 0, 0), damping(0.5f),
active(false), firstMove(true), remainingLife(0.0f), renderItem(nullptr)
{

}
Particle::Particle(Vector3D<float> p, Vector3D<float> v, float size, float mass, Vector4 color)
	: pose(physx::PxTransform(p)), ogPos(p), vel(v), acc(Vector3D<float>(0.0f, -10.0f, 0.0f)),
	damping(0.5f), real_Mass(mass), active(true), firstMove(true), renderItem(nullptr)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(size));
	renderItem = new RenderItem(shape, &pose, color);
}

Particle::~Particle()
{
	if (renderItem) {

		renderItem->release();

		renderItem = nullptr;

	}
}
void Particle::integrateEuler(float t)
{
	firstMove = false;
	pose.p += vel * t;
	vel += acc * t;

}


void Particle::integrateVerlet(float t)
{

	if (firstMove) {
		integrateEuler(t);
	}

	else {

		const physx::PxTransform aux = pose;
		pose.p = (pose.p * 2.0f) - antPos.p + (acc * t * t);
		antPos = aux;
	}

}


void Particle::integrateEulerSemi(float t)
{
	float massFactor = (real_Mass > 0.0001f) ? (1.0f / real_Mass) : 1.0f;
	Vector3D<float> effectiveAcc = acc + (summatoryForce * massFactor);

	vel += effectiveAcc * t;
	vel *= std::pow(damping, t);
	pose.p += vel * t;
}

void Particle::update(float dt)
{
	if (!active) return;

	remainingLife -= dt;
	if (remainingLife <= 0.0f) {
		deactivate();
		return;
	}


	// Integrar usando la aceleración calculada para este frame
	integrateEulerSemi(dt);

	// Resetear fuerzas acumuladas para el siguiente frame
	clearForce();
}
void Particle::setOriginalPosition()
{
	pose.p = physx::PxVec3(ogPos);

}

void Particle::createRenderItem(float size, Vector4 color)
{
	if (!renderItem) {

		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(size));
		renderItem = new RenderItem(shape, &pose, color);

	}

}
void Particle::deactivate()
{
	// Mover lejos o desactivar el renderizado para que no se dibuje

	active = false;

	if (renderItem) {

		renderItem->release(); // Elimina el elemento de la escena de renderizado

		renderItem = nullptr;

	}
}