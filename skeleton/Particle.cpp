#include "Particle.h"

Particle::Particle(Vector3D<float> p, Vector3D<float> v, float size,Vector4 color): vel(v),ogPos(p),damping(0.5f)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(size));
	pose = physx::PxTransform(p);
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

