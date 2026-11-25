#include "Scene5.h"


void Scene5::init()
{
	Particle* p1 = new Particle(Vector3D<float>(50.0f, 0.0f, 0.0f), Vector3D<float>(0.0f, 0.0f, 0.0f), 5.0f, 10.0f, Particle::TYPE::WITHOUT_GRAVITY);
	Particle* p2 = new Particle(Vector3D<float>(0.0f, 0.0f, 0.0f), Vector3D<float>(0.0f, 0.0f, 0.0f), 5.0f, 5.0f, Particle::TYPE::WITHOUT_GRAVITY);

	// Evitar que mueran a los 5 segundos (valor por defecto en tu constructor)
	p1->setLifeTime(99999.0f);
	p2->setLifeTime(99999.0f);

	m_particles.push_back(p1);
	m_particles.push_back(p2);


	Particle* p3 = new Particle(Vector3D<float>(10.0f, 20.0f, 0.0f), Vector3D<float>(0.0f, 0.0f, 0.0f), 3.0f, 5.0f, Particle::TYPE::MOVING);
	p3->setLifeTime(99999.0f);
	p3->setDamping(0.95f);
	m_particles.push_back(p3);

	m_anchorPose = physx::PxTransform(physx::PxVec3(0.0f, 35.0f, 0.0f));
	physx::PxShape* boxShape = CreateShape(physx::PxBoxGeometry(2.0f, 2.0f, 2.0f));
	m_anchorRenderItem = new RenderItem(boxShape, &m_anchorPose, Vector4(0.9f, 0.2f, 0.2f, 1.0f));

	m_ForceSystem = new ForceSystem(m_particles);

	// Un único emisor gestiona la interacción mutua entre p1 y p2
	SpringForceEmissor* spring = new SpringForceEmissor(1.0, 10.0, p1, p2);
	m_ForceSystem->addForceEmmiter(spring);

	AnchoredSpringForceEmissor* anchoredSpring = new AnchoredSpringForceEmissor(
		20.0,
		12.0,
		p3,
		&m_anchorPose,
		Vector3D<float>(0.0f, -2.0f, 0.0f),
		8,
		0.5f
	);
	m_ForceSystem->addForceEmmiter(anchoredSpring);

}

void Scene5::update(double dt)
{
	if (m_ForceSystem) {
		m_ForceSystem->update(dt);
	}

	// 2. Integrar movimiento y limpiar fuerzas acumuladas
	for (auto* p : m_particles) {
		if (p && p->isActive()) {
			p->update(dt);
		}
	}
}

void Scene5::keyPress(unsigned char key, const physx::PxTransform& camera)
{

}

void Scene5::cleanup()
{
	if (m_ForceSystem) {
		delete m_ForceSystem; // Esto también libera automáticamente el SpringForceEmissor
		m_ForceSystem = nullptr;
	}

	for (auto& a : m_particles) {
		delete a;
		a = nullptr;
	}
	m_particles.clear();

	if (m_anchorRenderItem) {
		m_anchorRenderItem->release();
		m_anchorRenderItem = nullptr;
	}
}
