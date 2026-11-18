#include "Scene4.h"


void Scene4::init()
{
	Vector3D<float> gravity(0.0f, -9.8f, 0.0f);
	Vector3D<float> minBounds(-50.0f, -5.0f, -50.0f);
	Vector3D<float> maxBounds(200.0f, 200.0f, 200.0f);
	float pSize = 1.0f;
	float lifeT = 5.0f;
	float freq = 30.0f;
	Vector4 pColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	m_particleSystem = new ParticleSystem(gravity, minBounds, maxBounds, Particle::TYPE::MOVING);
	m_ForceSystem = new ForceSystem(m_particleSystem);

	// Emisor desplazado ligeramente del origen para salir del ojo del torbellino
	Vector3D<float> pos(0.0f, 0.0f, 0.0f);
	Vector3D<float> dir(0.0f, 1.0f, 0.0f);
    
    //----- Generador Gravitacional -------------
	Emissor* emissor1 = new Emissor( m_particleSystem,
		pos,
		dir,
		new CilinderGeometry(pos, dir, 5.0f, 10.0f, 5.0f), // Base con radio para dispersar partículas
		pSize,
		lifeT,
		pColor,
		20.0f, 30.0f,
		0.5f,
		0.15f,
		freq,
		true,
		Emissor::DENSITYFUNTION::UNIFORM
	);
	m_particleSystem->addEmissor(emissor1);

	Vector3D<float> forcePos(0.0f, 15.0f, 0.0f);
	Vector3D<float> forceDir(.0f, 1.0f, 0.0f);

	//// Emisor de fuerza 
	Vector3D<float> forceVectorDir(1.0f, 0.0f, 0.0f);

	GravityForceEmissor* gravityEmissor = new GravityForceEmissor(
		m_ForceSystem,
		forcePos,
		forceVectorDir,
           new SphereGeometry(forcePos, forceDir, 10.0f),
		1.0f, // Magnitud para provocar un desvío visible e inmediato
		true,
		GravityForceEmissor::GravityType::Point
	);
	m_ForceSystem->addForceEmmiter(gravityEmissor);

    //------ Generador de Aire -------------
	pos = Vector3D<float>(0.0f, 0.0f, 50.0f);
	Emissor* emissor2 = new Emissor(m_particleSystem,
		pos,
		dir,
		new CilinderGeometry(pos, dir, 5.0f, 10.0f, 5.0f), // Base con radio para dispersar partículas
		pSize,
		lifeT,
		pColor,
		20.0f, 30.0f,
		0.5f,
		0.15f,
		freq,
		true,
		Emissor::DENSITYFUNTION::UNIFORM
	);
	m_particleSystem->addEmissor(emissor2);
	// Área de viento centrada en Y = 10.0
	Vector3D<float> windAreaPos(0.0f, 10.0f, 50.0f);
	Vector3D<float> windAreaDir(1.0f, 0.0f, .0f);
	BoxGeometry* windVolume = new BoxGeometry(windAreaPos, 
		windAreaDir, 
		20.0f, 
		15.0f, 
		20.0f);
	AirForceEmissor* airEmissor = new AirForceEmissor(
		m_ForceSystem, 
		windAreaPos, 
		windAreaDir, 
		windVolume,
		Vector3D<float>(40.0f, 0.0f, 0.0f), // Viento hacia +X
		1.5f, 
		0.0f, 
		true
	);
	m_ForceSystem->addForceEmmiter(airEmissor);

	//------ Generador de Torbellino -------------
	pos = Vector3D<float>(0.0f, 0.0f, -50.0f);
	freq = 500.0f;
	Emissor* emissor3 = new Emissor(m_particleSystem,
		pos,
		dir,
		new CilinderGeometry(pos, dir, 10.0f, 10.0f, 10.0f), // Base con radio para dispersar partículas
		pSize,
		lifeT,
		pColor,
		20.0f, 30.0f,
		0.5f,
		0.15f,
		freq,
		true,
		Emissor::DENSITYFUNTION::UNIFORM
	);
	m_particleSystem->addEmissor(emissor3);
	
	Vector3D<float> typhoonPos(0.0f, 0.0f, -50.0f);
	Vector3D<float> typhoonDir(0.0f, 1.0f, 0.0f);
	CilinderGeometry* typhoonVolume = new CilinderGeometry(
		typhoonPos, 
		typhoonDir, 
		25.0f, 
		50.0f, 
		35.0f);

	float K = 5.0f;   // Intensidad de giro del torbellino
	float k1 = 2.0f;  // Rozamiento lineal aerodinámico
	float k2 = 0.0f;  // Flujo laminar

	TyphoonForceEmissor* typhoon = new TyphoonForceEmissor(
		m_ForceSystem,
		typhoonPos,
		typhoonDir,
		typhoonVolume,
		0.6f, // K moderado para que giren sin salir disparadas arriba
		0.8f, // k1
		0.0f, // k2
		true
	);
	m_ForceSystem->addForceEmmiter(typhoon);

	//------ Generador de Explosión -------------
	freq = 60.0f;
	pos = Vector3D<float>(0.0f, 0.0f, 100.0f);
	Emissor* emissor4 = new Emissor(m_particleSystem,
		pos,
		dir,
		new CilinderGeometry(pos, dir, 5.0f, 10.0f, 5.0f), // Base con radio para dispersar partículas
		pSize,
		lifeT,
		pColor,
		20.0f, 30.0f,
		0.5f,
		0.15f,
		freq,
		true,
		Emissor::DENSITYFUNTION::UNIFORM
	);
	m_particleSystem->addEmissor(emissor4);
	pos = Vector3D<float>(0.0,0.0,100.0f);
	float explosionRadius = 35.0f;
	K = 8000.0f; // Fuerza inicial
	float tau = 0.4f;   // Decaimiento rápido en 1.6 segundos


	m_explosionEmissor = new ExplosionForceEmissor(
		m_ForceSystem,
		pos,
		new SphereGeometry(pos, Vector3D<float>(0, 1, 0), explosionRadius),
		K,
		tau,
		explosionRadius,
		false // Inicia inactivo hasta presionar la tecla
	);

	m_ForceSystem->addForceEmmiter(m_explosionEmissor);
}

void Scene4::update(double dt)
{
    if (m_ForceSystem) {
        m_ForceSystem->update(dt);
    }

    // 2. ParticleSystem actualiza sus emisores (m_fountainEmitter) e integra las partículas
    if (m_particleSystem) {
        m_particleSystem->update(dt);
    }
}

void Scene4::keyPress(unsigned char key, const physx::PxTransform& camera)
{
	if (key == 'b' || key == 'B') {
		if (m_explosionEmissor) {
			m_explosionEmissor->explode();
		}
	}
	if (key == 'r' || key == 'R') {
		m_particleSystem->onOffAllEmission();
	}
}

void Scene4::cleanup()
{
	if (m_ForceSystem) {
		delete m_ForceSystem;
		m_ForceSystem = nullptr;
	}

	if (m_particleSystem) {
		delete m_particleSystem;
		m_particleSystem = nullptr;
	}
}
