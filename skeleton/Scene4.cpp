#include "Scene4.h"


void Scene4::init()
{
	Vector3D<float> gravity(0.0f, -9.8f, 0.0f);
	Vector3D<float> minBounds(-50.0f, -5.0f, -50.0f);
	Vector3D<float> maxBounds(50.0f, 100.0f, 50.0f);
	float pSize = 1.0f;
	float lifeT = 5.0f;
	float freq = 60.0f;
	Vector4 pColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	m_particleSystem = new ParticleSystem(gravity, minBounds, maxBounds);
	m_ForceSystem = new ForceSystem(m_particleSystem);

	// 1. Emisor desplazado ligeramente del origen para salir del ojo del torbellino
	Vector3D<float> pos(0.0f, 0.0f, 0.0f);
	Vector3D<float> dir(0.0f, 1.0f, 0.0f);

	// Velocidad de salida adecuada para Proyectile (200 - 300 m/s equivalen a 20 - 30 m/s reales)
	//m_fountainEmitter = new Emissor(
	//	m_particleSystem,
	//	pos,
	//	dir,
	//	new CilinderGeometry(pos, dir, 3.0f, 5.0f, 3.0f), // Base con radio para dispersar partículas
	//	pSize,
	//	lifeT,
	//	pColor,
	//	20.0f, 40.0f,
	//	Emissor::TYPE::PROYECTILE,
	//	0.5f,
	//	0.15f,
	//	freq,
	//	true,
	//	Emissor::DENSITYFUNTION::UNIFORM
	//);
	//m_particleSystem->addEmissor(m_fountainEmitter);


	// ----- Generador particulas en posiciones aleatorias -------------
	//std::mt19937 rng(1337); 
	//std::uniform_real_distribution<float> distPos(-15.0f, 15.0f);
	//std::uniform_real_distribution<float> distMass(0.5f, 10.0f);

	//for (int i = 0; i < 40; ++i) {
	//	Vector3D<float> pPos(distPos(rng), 10.0f + std::abs(distPos(rng)) * 0.2f, distPos(rng));
	//	Vector3D<float> pVel(0.0f, 0.0f, 0.0f);
	//	float mass = distMass(rng);
	//	float size = 0.3f + mass * 0.1f; // Tamaño proporcional a la masa

	//	// Partícula de color dinámico según su masa (rojas ligeras, azules pesadas)
	//	Vector4 color(1.0f - mass / 10.0f, 0.2f, mass / 10.0f, 1.0f);

	//	m_particleSystem->addParticle(pPos, pVel, mass, size, 15.0f, color);
	//}
    
    //----- Generador Gravitacional -------------
	//Vector3D<float> forcePos(0.0f, 15.0f, 0.0f);
	//Vector3D<float> forceDir(.0f, 0.0f, 1.0f);

	////// Emisor de fuerza 
	//Vector3D<float> forceVectorDir(1.0f, 0.0f, 0.0f);

	//GravityForceEmissor* gravityEmissor = new GravityForceEmissor(
	//	m_ForceSystem,
	//	forcePos,
	//	forceVectorDir,
    //       new SphereGeometry(forcePos, forceDir, 10.0f),
	//	1.0f, // Magnitud para provocar un desvío visible e inmediato
	//	true,
	//	GravityForceEmissor::GravityType::Point
	//);
	//m_ForceSystem->addForceEmmiter(m_GrvityEmissor);

    //------ Generador de Aire -------------

	// Área de viento centrada en Y = 10.0
	//Vector3D<float> windAreaPos(0.0f, 20.0f, 0.0f);
	//Vector3D<float> windAreaDir(.0f, .0f, 1.0f);
	//BoxGeometry* windVolume = new BoxGeometry(windAreaPos, windAreaDir, 20.0f, 10.0f, 20.0f);

	//// Viento constante hacia la derecha (+X)
	//Vector3D<float> windVelocity(.0f, .0f, 20.0f);
	//float k1 = 1.5f; // Rozamiento lineal
	//float k2 = 0.0f; // Flujo laminar

	//AirForceEmissor* airEmissor = new AirForceEmissor(
	//	m_ForceSystem, windAreaPos, windAreaDir, windVolume, windVelocity, k1, k2, true
	//);
	//m_ForceSystem->addForceEmmiter(airEmissor);

	//------ Generador de Torbellino -------------
	//Vector3D<float> typhoonPos(0.0f, 0.0f, 0.0f);
	//Vector3D<float> typhoonDir(0.0f, 1.0f, 0.0f);
	//CilinderGeometry* typhoonVolume = new CilinderGeometry(typhoonPos, typhoonDir, 25.0f, 50.0f, 25.0f);

	//float K = 3.0f;   // Intensidad de giro del torbellino
	//float k1 = 2.0f;  // Rozamiento lineal aerodinámico
	//float k2 = 0.0f;  // Flujo laminar

	//TyphoonForceEmissor* typhoon = new TyphoonForceEmissor(
	//	m_ForceSystem, typhoonPos, typhoonDir, typhoonVolume, K, k1, k2, true
	//)
	//m_ForceSystem->addForceEmmiter(typhoon);

	//------ Generador de Explosión -------------
	//Vector3D<float> explosionCenter(0.0f, 0.0f, 0.0f);
	//float explosionRadius = 35.0f;
	//float K = 8000.0f; // Fuerza inicial
	//float tau = 0.4f;   // Decaimiento rápido en 1.6 segundos

	//SphereGeometry* explosionSphere = new SphereGeometry(explosionCenter, Vector3D<float>(0, 1, 0), explosionRadius);

	//m_explosionEmissor = new ExplosionForceEmissor(
	//	m_ForceSystem,
	//	explosionCenter,
	//	explosionSphere,
	//	K,
	//	tau,
	//	explosionRadius,
	//	false // Inicia inactivo hasta presionar la tecla
	//);

	//m_ForceSystem->addForceEmmiter(m_explosionEmissor);
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
			m_explosionEmissor->explode(Vector3D<float>(0.0f, 0.0f, 0.0f));
		}
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
		m_fountainEmitter = nullptr;
	}
}
