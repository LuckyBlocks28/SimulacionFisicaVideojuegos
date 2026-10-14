#include "Scene2.h"
#include "Proyectile.h"
#include "RenderUtils.hpp"

void Scene2::init()
{
}

void Scene2::update(double dt)
{
	for (auto& m_particle : m_proyectiles) {
		m_particle->integrateEulerSemi(dt);
	}
}

void Scene2::keyPress(unsigned char key, const physx::PxTransform& camera)
{
	Camera* cam = __RENDER_UTILS_H__::GetCamera();
	Vector3D<float> shootDir = cam->getDir();
	Vector3D<float> camPos = cam->getEye() + shootDir * 1.5f;
	if (key == 'r' || key == 'R') {
		cleanup();
		init();
	}
	else if (key == '+') {
		for (auto& m_particle : m_proyectiles) {
			masaPistola += 0.1f;
			masaTanque += 0.1f;
			masaCanon += 0.1f;
		}
	}
	else if (key == '-') {
		for (auto& m_particle : m_proyectiles) {
			masaPistola -= 0.1f;
			masaTanque -= 0.1f;
			masaCanon -= 0.1f;
		}
	}
	else if (key == 'e' || key == 'E') {
		m_proyectiles.push_back(new Proyectile(camPos, shootDir * 330.f, 5.0f, masaPistola, Vector4(0.0, 1.0, 0.0, 1.0)));
	}
	else if (key == 'q' || key == 'Q') {
		m_proyectiles.push_back(new Proyectile(camPos, shootDir * 2000.f, 5.0f, masaTanque, Vector4(0.0, 0.0, 1.0, 1.0)));
	}
	else if (key == 't' || key == 'T') {
		m_proyectiles.push_back(new Proyectile(camPos, shootDir * 500.f, 5.0f, masaCanon,Vector4(1.0,0.0,0.0,1.0)));
	}
}

void Scene2::cleanup()
{
	for (auto& m_particle : m_proyectiles) {
		delete m_particle;
		m_particle = nullptr;
	}
	m_proyectiles.clear();
}
