#include "Particle.h"

Particle::Particle()
	: pose(physx::PxTransform(physx::PxVec3(0, 0, 0))),
	antPos(physx::PxTransform(physx::PxVec3(0, 0, 0))),
	ogPos(0, 0, 0), vel(0, 0, 0), acc(0.0f, -9.8f, 0.0f),
	summatoryForce(0.0f, 0.0f, 0.0f), damping(0.99f), real_Mass(1.0f),
	active(false), firstMove(true), remainingLife(0.0f),
	renderItem(nullptr), m_type(TYPE::STATIC)
{
}

Particle::Particle(Vector3D<float> p, Vector3D<float> v, float size, float mass, TYPE type, Vector4 color)
	: pose(physx::PxTransform(p)), antPos(physx::PxTransform(p)),
	ogPos(p), vel(v), acc(0.0f, -9.8f, 0.0f),
	summatoryForce(0.0f, 0.0f, 0.0f), damping(0.99f), real_Mass(mass),
	active(true), firstMove(true), renderItem(nullptr),
	m_type(type), remainingLife(5.0f)
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
	if (m_type == TYPE::STATIC) return;

	Vector3D<float> effectiveAcc(0.0f, 0.0f, 0.0f);

	// MOVING y WITHOUT_GRAVITY reciben fuerzas externas
	if (m_type == TYPE::MOVING || m_type == TYPE::WITHOUT_GRAVITY) {
		float massFactor = (real_Mass > 0.0001f) ? (1.0f / real_Mass) : 1.0f;
		effectiveAcc += summatoryForce * massFactor;
	}

	// MOVING y WITHOUT_FORCE reciben gravedad
	if (m_type == TYPE::MOVING || m_type == TYPE::WITHOUT_FORCE) {
		effectiveAcc += acc;
	}

	vel += effectiveAcc * t;
	vel *= std::pow(damping, t);
	pose.p += vel * t;
}

void Particle::update(float dt)
{
	if (!active) return;
	if (m_type != TYPE::STATIC) {
		remainingLife -= dt;
		if (remainingLife <= 0.0f) {
			deactivate();
			return;
		}
		integrateEulerSemi(dt);
	}

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