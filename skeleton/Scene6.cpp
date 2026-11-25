#include "Scene6.h"

void Scene6::init()
{
	float boxHeight = 2.0f;
	float initialVolume = 0.015f;
	float initialMass = 10.0f;
	float waterY = 0.0f;

	m_floatingBox = new Particle(
		Vector3D<float>(0.0f, 10.0f, 0.0f),
		Vector3D<float>(0.0f, 0.0f, 0.0f),
		1.0f,
		initialMass,
		Particle::TYPE::MOVING
	);
	m_floatingBox->setLifeTime(99999.0f);
	// Subimos el damping de la partícula a 0.99f para que el frenado lo controle LiquidDragForceEmissor
	m_floatingBox->setDamping(0.99f);

	m_particles.push_back(m_floatingBox);

	m_ForceSystem = new ForceSystem(m_particles);

	// 1. Emisor de empuje hidrostático
	m_buoyancyEmitter = new BuoyancyForceEmissor(
		boxHeight,
		initialVolume,
		waterY,
		1000.0f,
		m_floatingBox
	);
	m_ForceSystem->addForceEmmiter(m_buoyancyEmitter);

	// 2. Emisor de rozamiento viscoso (k1 = 5.0 para agua, k2 = 0.0 para líquidos)
	m_dragEmitter = new LiquidDragForceEmissor(
		5.0f,
		0.0f,
		boxHeight,
		waterY,
		m_floatingBox
	);
	m_ForceSystem->addForceEmmiter(m_dragEmitter);

	setLiquid("Agua", 1000.0f, 5.0f, Vector4(0.1f, 0.4f, 0.9f, 0.6f));
}

void Scene6::setLiquid(const std::string& name, float density, float k1, Vector4 color)
{
	if (!m_buoyancyEmitter || !m_dragEmitter) return;

	m_buoyancyEmitter->setLiquidDensity(density);
	m_buoyancyEmitter->setWaterColor(color);
	m_dragEmitter->setCoefficients(k1, 0.0f); // k2 = 0 en líquidos

	std::cout << "Liquido seleccionado: " << name
		<< " | Densidad: " << density << " kg/m3"
		<< " | Viscosidad (k1): " << k1 << "\n";
}

void Scene6::update(double dt)
{
	if (m_ForceSystem) {
		m_ForceSystem->update(dt);
	}

	for (auto* p : m_particles) {
		if (p && p->isActive()) {
			p->update(static_cast<float>(dt));
		}
	}
}

void Scene6::keyPress(unsigned char key, const physx::PxTransform& camera)
{
	if (!m_floatingBox || !m_buoyancyEmitter || !m_dragEmitter) return;

	switch (toupper(key))
	{
		// Cambiar tipo de líquido (Teclas 1, 2, 3)
	case 'Q':
		setLiquid("Agua", 1000.0f, 5.0f, Vector4(0.1f, 0.4f, 0.9f, 0.6f));
		break;
	case 'T':
		setLiquid("Aceite", 850.0f, 25.0f, Vector4(0.85f, 0.75f, 0.1f, 0.6f));
		break;
	case 'E':
		setLiquid("Miel / Glicerina", 1350.0f, 90.0f, Vector4(0.8f, 0.4f, 0.05f, 0.7f));
		break;

		// Reiniciar posición del cubo arriba para observar cómo cae en cada líquido (Tecla R)
	case 'R':
		m_floatingBox->setPosition(Vector3D<float>(0.0f, 10.0f, 0.0f));
		m_floatingBox->setVelocity(Vector3D<float>(0.0f, 0.0f, 0.0f));
		break;

		// Modificar MASA del cubo (Teclas M / N)
	case 'M':
		m_floatingBox->setMass(m_floatingBox->getMass() + 2.0f);
		std::cout << "Masa: " << m_floatingBox->getMass() << " kg | Empuje max (kg eq): "
			<< m_buoyancyEmitter->getLiquidDensity() * m_buoyancyEmitter->getVolume() << " kg\n";
		break;
	case 'N':
		if (m_floatingBox->getMass() > 2.0f) {
			m_floatingBox->setMass(m_floatingBox->getMass() - 2.0f);
			std::cout << "Masa: " << m_floatingBox->getMass() << " kg | Empuje max (kg eq): "
				<< m_buoyancyEmitter->getLiquidDensity() * m_buoyancyEmitter->getVolume() << " kg\n";
		}
		break;

		// Modificar VOLUMEN del cubo (Teclas V / B)
	case 'V':
		m_buoyancyEmitter->setVolume(m_buoyancyEmitter->getVolume() + 0.002f);
		std::cout << "Volumen: " << m_buoyancyEmitter->getVolume() << " m3 | Empuje max (kg eq): "
			<< m_buoyancyEmitter->getLiquidDensity() * m_buoyancyEmitter->getVolume() << " kg\n";
		break;
	case 'B':
		m_buoyancyEmitter->setVolume(m_buoyancyEmitter->getVolume() - 0.002f);
		std::cout << "Volumen: " << m_buoyancyEmitter->getVolume() << " m3 | Empuje max (kg eq): "
			<< m_buoyancyEmitter->getLiquidDensity() * m_buoyancyEmitter->getVolume() << " kg\n";
		break;
	}
}

void Scene6::cleanup()
{
	if (m_ForceSystem) {
		delete m_ForceSystem; // Libera automáticamente m_buoyancyEmitter y m_dragEmitter
		m_ForceSystem = nullptr;
		m_buoyancyEmitter = nullptr;
		m_dragEmitter = nullptr;
	}

	for (auto& p : m_particles) {
		delete p;
		p = nullptr;
	}
	m_particles.clear();
	m_floatingBox = nullptr;
}