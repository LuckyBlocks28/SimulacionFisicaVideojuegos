#include "Scene5.h"


void Scene5::init()
{
	//Particulas "atadas" con un muelle
	Particle* p1 = new Particle(Vector3D<float>(50.0f, 0.0f, 0.0f), Vector3D<float>(0.0f, 0.0f, 0.0f), 5.0f, 10.0f, Particle::TYPE::WITHOUT_GRAVITY);
	Particle* p2 = new Particle(Vector3D<float>(0.0f, 0.0f, 0.0f), Vector3D<float>(0.0f, 0.0f, 0.0f), 5.0f, 5.0f, Particle::TYPE::WITHOUT_GRAVITY);

	// Evitar que mueran a los 5 segundos (valor por defecto en tu constructor)
	p1->setLifeTime(99999.0f);
	p2->setLifeTime(99999.0f);

	m_particles.push_back(p1);
	m_particles.push_back(p2);

	//Particula anclada a un cubo
	Particle* p3 = new Particle(Vector3D<float>(10.0f, 20.0f, 0.0f), Vector3D<float>(0.0f, 0.0f, 0.0f), 3.0f, 5.0f, Particle::TYPE::MOVING);
	p3->setLifeTime(99999.0f);
	p3->setDamping(0.95f);
	m_particles.push_back(p3);

	m_anchorPose = physx::PxTransform(physx::PxVec3(0.0f, 35.0f, 0.0f));
	physx::PxShape* boxShape = CreateShape(physx::PxBoxGeometry(2.0f, 2.0f, 2.0f));
	m_anchorRenderItem = new RenderItem(boxShape, &m_anchorPose, Vector4(0.9f, 0.2f, 0.2f, 1.0f));

	//	----------------------------------
	//			Simulación pinball
	//	----------------------------------
	m_pinballAnchorPose = physx::PxTransform(physx::PxVec3(0.0f, m_pinballY, 0.0f));
	physx::PxShape* pinballWallShape = CreateShape(physx::PxBoxGeometry(1.5f, 3.5f, 3.5f));
	m_pinballAnchorRenderItem = new RenderItem(pinballWallShape, &m_pinballAnchorPose, Vector4(0.8f, 0.15f, 0.15f, 1.0f));

	// Partícula tope del muelle (naranja)
	m_plungerHead = new Particle(
		Vector3D<float>(m_restX, m_pinballY, 0.0f),
		Vector3D<float>(0.0f, 0.0f, 0.0f),
		m_plungerRadius,
		2.0f,
		Particle::TYPE::WITHOUT_GRAVITY,
		Vector4(0.95f, 0.6f, 0.1f, 1.0f)
	);
	m_plungerHead->setLifeTime(99999.0f);
	m_plungerHead->setDamping(0.92f);
	m_particles.push_back(m_plungerHead);

	// Partícula bola de pinball (azul claro), situada justo delante del tope
	m_pinballBall = new Particle(
		Vector3D<float>(m_restX + m_plungerRadius + m_ballRadius + 10.0f, m_pinballY, 0.0f),
		Vector3D<float>(0.0f, 0.0f, 0.0f),
		m_ballRadius,
		1.0f,
		Particle::TYPE::WITHOUT_GRAVITY,
		Vector4(0.2f, 0.7f, 1.0f, 1.0f)
	);
	m_pinballBall->setLifeTime(99999.0f);
	m_pinballBall->setDamping(0.995f);
	m_particles.push_back(m_pinballBall);

	//Crear sistema de fuerzas
	m_ForceSystem = new ForceSystem(m_particles);

	// Un único emisor gestiona la interacción mutua entre p1 y p2
	SpringForceEmissor* spring = new SpringForceEmissor(1.0, 10.0, p1, p2);
	m_ForceSystem->addForceEmmiter(spring);

	//Generador de fuerza para partícula anclada
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

	m_plungerSpring = new AnchoredSpringForceEmissor(
		120.0,
		m_restX,
		m_plungerHead,
		&m_pinballAnchorPose,
		Vector3D<float>(1.5f, 0.0f, 0.0f),
		8,
		0.5f
	);
	m_ForceSystem->addForceEmmiter(m_plungerSpring);
}

void Scene5::update(double dt)
{
	float fdt = dt;

	// Detectar si se ha soltado la tecla ESPACIO para liberar el muelle del pinball
	if (m_isCharging) {
		m_timeSinceKey += fdt;
		if (m_timeSinceKey > 0.12f) {
			m_isCharging = false;
			if (m_plungerSpring) {
				m_plungerSpring->setActive(true);
			}
		}
	}

	if (m_ForceSystem) {
		m_ForceSystem->update(dt);
	}

	// Integrar movimiento y limpiar fuerzas acumuladas
	for (auto* p : m_particles) {
		if (p && p->isActive()) {
			p->update(fdt);
		}
	}

	// Colisión/empuje entre el tope del muelle y la bola del pinball al disparar
	if (!m_isCharging && m_plungerHead && m_pinballBall) {
		float minDistance = m_plungerRadius + m_ballRadius;
		float currentDist = m_pinballBall->getPos().getX() - m_plungerHead->getPos().getX();

		if (currentDist <= minDistance && m_plungerHead->getVel().getX() > m_pinballBall->getVel().getX()) {
			// Evitar que se solapen
			Vector3D<float> ballPos = m_pinballBall->getPos();
			ballPos.setX(m_plungerHead->getPos().getX() + minDistance + 10.0f);
			m_pinballBall->setPosition(ballPos);

			// Impulsar la bola con la velocidad que lleva el tope del muelle
			m_pinballBall->setVelocity(m_plungerHead->getVel());
		}
	}
}

void Scene5::keyPress(unsigned char key, const physx::PxTransform& camera)
{
	if (!m_plungerHead || !m_pinballBall || !m_plungerSpring) return;

	switch (toupper(key))
	{
		// Mantener ESPACIO para comprimir el muelle hacia atrás
	case ' ':
		m_isCharging = true;
		m_timeSinceKey = 0.0f;

		m_plungerSpring->setActive(false);
		m_plungerHead->setVelocity(Vector3D<float>(0.0f, 0.0f, 0.0f));

		{
			Vector3D<float> pos = m_plungerHead->getPos();
			if (pos.getX() > m_maxPullbackX) {
				pos.setX(pos.getX() - 0.6f);
				m_plungerHead->setPosition(pos);
			}
		}
		break;

		// Pulsar R para resetear la bola y el lanzador
	case 'R':
		m_isCharging = false;
		m_plungerSpring->setActive(true);
		m_plungerHead->setPosition(Vector3D<float>(m_restX, m_pinballY, 0.0f));
		m_plungerHead->setVelocity(Vector3D<float>(0.0f, 0.0f, 0.0f));
		m_pinballBall->setPosition(Vector3D<float>(m_restX + m_plungerRadius + m_ballRadius, m_pinballY, 0.0f));
		m_pinballBall->setVelocity(Vector3D<float>(0.0f, 0.0f, 0.0f));
		break;
	}
}

void Scene5::cleanup()
{
	if (m_ForceSystem) {
		delete m_ForceSystem; // Libera automáticamente spring, anchoredSpring y m_plungerSpring
		m_ForceSystem = nullptr;
		m_plungerSpring = nullptr;
	}

	for (auto& a : m_particles) {
		delete a;
		a = nullptr;
	}
	m_particles.clear();
	m_plungerHead = nullptr;
	m_pinballBall = nullptr;

	if (m_anchorRenderItem) {
		m_anchorRenderItem->release();
		m_anchorRenderItem = nullptr;
	}

	if (m_pinballAnchorRenderItem) {
		m_pinballAnchorRenderItem->release();
		m_pinballAnchorRenderItem = nullptr;
	}
}
