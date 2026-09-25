#include "Scene1.h"

void Scene1::init()
{
	prueba = new Particle(Vector3D<float>(0.0f,10.0f,0.0f), Vector3D<float>(10.0f, 0.0f, 0.0f), Vector3D<float>(10.0f, 0.0f, 0.0f), 5.0f);
}

void Scene1::update(double dt)
{
	//prueba->integrateEuler(dt);
	prueba->integrateEulerSemi(dt);
}

void Scene1::keyPress(unsigned char key, const physx::PxTransform& camera)
{
	if (key == 'r' || key == 'R') {
		prueba->setOriginalPosition();
	}
}

void Scene1::cleanup()
{
	if (prueba) {
		prueba->release();
	}
}
