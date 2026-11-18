#include "Scene3.h"
#include "PrismGeometry.h"
#include "CilinderGeometry.h"
#include "SphereGeometry.h"
#include "BoxGeometry.h"

void Scene3::init()
{
    Vector3D<float> gravity(0.0f, -9.8f, 0.0f);
    Vector3D<float> minBounds(-50.0f, -50.0f, -50.0f);
    Vector3D<float> maxBounds(100.0f, 100.0f, 100.0f);
    float pSize = 1.0f;
    float lifeT = 5.0f;
    float freq = 100.0f;
    Vector4 pColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

    m_particleSystem = new ParticleSystem(gravity, minBounds, maxBounds,Particle::TYPE::WITHOUT_GRAVITY);
 
    Vector3D<float> pos(0.0f, 0.0f, 0.0f);
    Vector3D<float> dir(0.f, 1.f, 0.f);

    Emissor* emissor1 = new Emissor(
        m_particleSystem, 
        pos,  //Posición
        dir,  //Dirección
        new CilinderGeometry(pos, dir, 10.0f, 10.0f, 10.0f), //Geometría
        pSize, //Tamaño de partícula
        lifeT, // Tiempo de vida
        pColor,  //Color de particula
        10.0f, 20.0f, // Velocidad mín y máx de salida
        0.5f, // Media
        0.15f, //Desviación típica
        freq, // Frecuencia
        true, // Activo desde el inicio
        Emissor::DENSITYFUNTION::UNIFORM //Tipo de distribución
    ); 
    m_particleSystem->addEmissor(emissor1);

    pos = Vector3D<float>(0.0f, 0.0f, -50.0f);
    Emissor* emissor2 = new Emissor(
        m_particleSystem,
        pos,  //Posición
        dir,  //Dirección
        new PrismGeometry(pos, dir, 3.0f, 3.0f, 30.0f,10.0f,10.0f), //Geometría
        pSize, //Tamaño de partícula
        lifeT, // Tiempo de vida
        pColor, //Color de particula
        10.0f, 20.0f,
        0.0f, // Media
        1.0f, //Desviación típica
        freq, // Frecuencia
        true, // Activo desde el inicio
        Emissor::DENSITYFUNTION::UNIFORM //Tipo de distribución
    );
    m_particleSystem->addEmissor(emissor2);

    pos = Vector3D<float>(0.0f, 0.0f, 50.0f);
    freq = 500.0f;
    Emissor* emissor3 = new Emissor(
        m_particleSystem,
        pos,  //Posición
        dir,  //Dirección
        new BoxGeometry(pos, dir, 20.0f, 20.0f, 20.0f), //Geometría
        pSize, //Tamaño de partícula
        lifeT, // Tiempo de vida
        pColor,  //Color de particula
        10.0f, 20.0f, // Velocidad mín y máx de salida
        0.5f, // Media
        1.0f, //Desviación típica
        freq, // Frecuencia
        true, // Activo desde el inicio
        Emissor::DENSITYFUNTION::UNIFORM //Tipo de distribución
    );
    emissor3->setKeepInside(true);
    m_particleSystem->addEmissor(emissor3);

    pos = Vector3D<float>(50.0f, 0.0f, 0.0f);
    Emissor* emissor4 = new Emissor(
        m_particleSystem,
        pos,  //Posición
        dir,  //Dirección
        new SphereGeometry(pos, dir, 20.0f), //Geometría
        pSize, //Tamaño de partícula
        lifeT, // Tiempo de vida
        pColor,  //Color de particula
        10.0f, 20.0f, // Velocidad mín y máx de salida
        0.0f, // Media
        1.0f, //Desviación típica
        freq, // Frecuencia
        true, // Activo desde el inicio
        Emissor::DENSITYFUNTION::UNIFORM //Tipo de distribución
    );
    emissor4->setKeepInside(true);
    m_particleSystem->addEmissor(emissor4);
}

void Scene3::update(double dt)
{
    if (m_particleSystem) {
        m_particleSystem->update(dt);
    }
}

void Scene3::keyPress(unsigned char key, const physx::PxTransform& camera)
{
    if (key == 'r' || key == 'R') {
        m_particleSystem->onOffAllEmission();
    }      
}

void Scene3::cleanup()
{
    if (m_particleSystem) {
        delete m_particleSystem;
        m_particleSystem = nullptr;
        m_fountainEmitter = nullptr;
    }
}
