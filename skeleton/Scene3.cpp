#include "Scene3.h"
#include "PrismGeometry.h"
#include "CilinderGeometry.h"
#include "SphereGeometry.h"
#include "BoxGeometry.h"

void Scene3::init()
{
    Vector3D<float> gravity(0.0f, -9.8f, 0.0f);
    Vector3D<float> minBounds(-50.0f, -5.0f, -50.0f);
    Vector3D<float> maxBounds(50.0f, 100.0f, 50.0f);
    float pSize = 1.0f;
    float lifeT = 5.0f;
    float freq = 100.0f;
    Vector4 pColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

    m_particleSystem = new ParticleSystem(gravity, minBounds, maxBounds);
 
    Vector3D<float> pos(0.0f, 0.0f, 0.0f);
    Vector3D<float> dir(0.f, 1.f, 0.f);

    //m_fountainEmitter = new Emissor(
    //    m_particleSystem, 
    //    pos,  //Posición
    //    dir,  //Dirección
    //    new CilinderGeometry(pos, dir, 1.0f, 10.0f, 20.0f), //Geometría
    //    pSize, //Tamaño de partícula
    //    lifeT, // Tiempo de vida
    //    pColor,  //Color de particula
    //    10.0f, 20.0f, // Velocidad mín y máx de salida
    //    Emissor::TYPE::PROYECTILE, //Tipo de proyectil
    //    0.5f, // Media
    //    0.15f, //Desviación típica
    //    freq, // Frecuencia
    //    true, // Activo desde el inicio
    //    Emissor::DENSITYFUNTION::UNIFORM //Tipo de distribución
    //); 

    m_fountainEmitter = new Emissor(
        m_particleSystem,
        pos,  //Posición
        dir,  //Dirección
        new PrismGeometry(pos, dir, 10.0f, 12.0f, 10.0f,10.0f,12.0f), //Geometría
        pSize, //Tamaño de partícula
        lifeT, // Tiempo de vida
        pColor,  //Color de particula
        10.0f, 40.0f, // Velocidad mín y máx de salida
        Emissor::TYPE::PROYECTILE, //Tipo de proyectil
        0.0f, // Media
        1.0f, //Desviación típica
        freq, // Frecuencia
        true, // Activo desde el inicio
        Emissor::DENSITYFUNTION::NORMAL //Tipo de distribución
    );
    
    //m_fountainEmitter = new Emissor(
    //    m_particleSystem,
    //    pos,  //Posición
    //    dir,  //Dirección
    //    new BoxGeometry(pos, dir, 3.0f, 3.0f, 3.0f), //Geometría
    //    pSize, //Tamaño de partícula
    //    lifeT, // Tiempo de vida
    //    pColor,  //Color de particula
    //    10.0f, 40.0f, // Velocidad mín y máx de salida
    //    Emissor::TYPE::PROYECTILE, //Tipo de proyectil
    //    0.5f, // Media
    //    1.0f, //Desviación típica
    //    freq, // Frecuencia
    //    true, // Activo desde el inicio
    //    Emissor::DENSITYFUNTION::NORMAL //Tipo de distribución
    //);

    //m_fountainEmitter = new Emissor(
    //    m_particleSystem,
    //    pos,  //Posición
    //    dir,  //Dirección
    //    new SphereGeometry(pos, dir, 5.0f), //Geometría
    //    pSize, //Tamaño de partícula
    //    lifeT, // Tiempo de vida
    //    pColor,  //Color de particula
    //    10.0f, 40.0f, // Velocidad mín y máx de salida
    //    Emissor::TYPE::PROYECTILE, //Tipo de proyectil
    //    0.0f, // Media
    //    1.0f, //Desviación típica
    //    freq, // Frecuencia
    //    true, // Activo desde el inicio
    //    Emissor::DENSITYFUNTION::NORMAL //Tipo de distribución
    //);


    // Seleccionar distribución uniforme o normal
    m_fountainEmitter->setDensityFunction(Emissor::NORMAL);

    // Registrar el emisor en el sistema
    // (ParticleSystem se encarga de llamar a update y liberar la memoria al destruirse)
    m_particleSystem->addEmissor(m_fountainEmitter);
}

void Scene3::update(double dt)
{
    if (m_particleSystem) {
        m_particleSystem->update(dt);
    }
}

void Scene3::keyPress(unsigned char key, const physx::PxTransform& camera)
{
    if (!m_particleSystem || !m_fountainEmitter) return;

    switch (key)
    {
        // Activar / Desactivar la fuente
    case 't':
    case 'T':
        m_fountainEmitter->activate(!m_fountainEmitter->isActive());
        break;

        // Cambiar entre distribución Uniforme y Gaussiana (Normal)
    case 'u':
    case 'U':
        m_fountainEmitter->setDensityFunction(Emissor::UNIFORM);
        break;

    case 'n':
    case 'N':
        m_fountainEmitter->setDensityFunction(Emissor::NORMAL);
        break;

        // Aumentar / Disminuir la frecuencia de emisión
    case '+':
        m_fountainEmitter->setFreq(m_fountainEmitter->getFreq() + 5.0f);
        break;

    case '-':
        m_fountainEmitter->setFreq((m_fountainEmitter->getFreq() - 5.0f) < 1.0f ? 1.0f : (m_fountainEmitter->getFreq() - 5.0f));
        break;
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
