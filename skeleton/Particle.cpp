#include "Particle.h"

Particle::Particle() : ogPos(0, 0, 0), vel(0, 0, 0), acc(0, 0, 0), damping(0.5f),
active(false), firstMove(true), remainingLife(0.0f), renderItem(nullptr)
{
}

Particle::Particle(Vector3D<float> p, Vector3D<float> v, float size, Vector4 color):pose (physx::PxTransform(p)), ogPos(p),vel(v), acc(Vector3D<float>(0.0,-10.0,0.0)),damping(0.5f),active(true),firstMove(true)
{
	if (renderItem) {
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(size));
		renderItem = new RenderItem(shape, &pose, color);
	}
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
void Particle::integrateEulerSemi(float t)
{
	vel += acc * t;
	vel *= pow(damping, t);
	pose.p += vel * t;
}
void Particle::integrateVerlet(float t)
{
	if (firstMove) {
		integrateEuler(t);
	}
	else{
		const physx::PxTransform aux = pose;
		pose.p = (pose.p * 2.0f) - antPos.p + (acc * t * t);
		antPos = aux;
	}
}
void Particle::setOriginalPosition()
{
	pose.p = physx::PxVec3(ogPos);	
}
void Particle::setDamping(float d)
{
	damping = d; 
}

void Particle::createRenderItem(float size, Vector4 color)
{
	if (!renderItem) {
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(size));
		renderItem = new RenderItem(shape, &pose, color);
	}
}

void Particle::update(float dt)
{
	if (!active) return;

	// Restar el tiempo de vida transcurrido
	remainingLife -= dt;
	if (remainingLife <= 0.0f) {
		deactivate();
		return;
	}

	// Integración física
	integrateEulerSemi(dt);
}
void Particle::deactivate()
{
	active = false;
	// Mover lejos o desactivar el renderizado para que no se dibuje
	active = false;
	if (renderItem) {
		renderItem->release(); // Elimina el elemento de la escena de renderizado
		renderItem = nullptr;
	}
}

