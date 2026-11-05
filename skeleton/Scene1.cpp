#include "Scene1.h"

void Scene1::init()
{
	prueba = new Particle(Vector3D<float>(0.0f,50.0f,0.0f), Vector3D<float>(0.0f, 0.0f, 40.0f), 5.0f,1.0f);
	prueba1 = new Particle(Vector3D<float>(0.0f,50.0f,30.0f), Vector3D<float>(0.0f, 0.0f, 40.0f), 5.0f,50.0f,Vector4(1.0f,0.0f,0.0f,1.0f));

}

void Scene1::update(double dt)
{
	//prueba->integrateEuler(dt);
	prueba->integrateEulerSemi(dt);
	prueba1->integrateEulerSemi(dt);
}

void Scene1::keyPress(unsigned char key, const physx::PxTransform& camera)
{
	if (key == 'r' || key == 'R') {
		prueba->setOriginalPosition();
		prueba1->setOriginalPosition();
	}
}

void Scene1::cleanup()
{
	if (prueba) {
		delete prueba;
		prueba = nullptr;
	}
}
