#include "Particle.h"

Particle::Particle(Vector3D<float> p, Vector3D<float> v, Vector3D<float> a, float size,Vector4 color): vel(v),acc(a),ogPos(p),damping(0.5f)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(size));
	pose = physx::PxTransform(p);

	renderItem = new RenderItem(shape, &pose, color);
}

Particle::~Particle() {}

void Particle::integrateEuler(float t)
{
	pose.p += vel * t;
	vel += acc * t;
}
void Particle::integrateEulerSemi(float t)
{
	vel += acc * t;
	vel *= pow(damping, t);
	pose.p += vel * t;
}
void Particle::setOriginalPosition()
{
	pose.p = physx::PxVec3(ogPos);	
}
void Particle::setDamping(float d)
{
	damping = d; 
}
void Particle::release() {
	if (renderItem) {
		renderItem->release();
		renderItem = nullptr;
	}
}
