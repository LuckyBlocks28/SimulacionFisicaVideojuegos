#include "Scene3.h"


void Scene3::init()
{
    Vector3D<float> gravity(0.0f, -9.8f, 0.0f);
    Vector3D<float> minBounds(-50.0f, -5.0f, -50.0f);
    Vector3D<float> maxBounds(50.0f, 100.0f, 50.0f);
    float pSize = 1.0f;
    float lifeT = 5.0f;
    float freq = 300.0f;
    Vector4 pColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

    m_particleSystem = new ParticleSystem(gravity, minBounds, maxBounds);
 
    Vector3D<float> pos(0.0f, 0.0f, 0.0f);
    Vector3D<float> dir(1.f, 0.f, 0.f);

    //m_fountainEmitter = Emissor::createRectangleGenerator(
    //    m_particleSystem,
    //    pos,
    //    dir,
    //    pSize,
    //    lifeT,
    //    pColor,
    //    10.0f, 
    //    5.0f, 5.0f,
    //    10.0f, 10.0f,
    //    10.0f, 40.0f,     // Velocidad mín y máx de salida
    //    Emissor::PROYECTILE,
    //    0.0f, 1.0f,      // Media y desviación típica (para distribución Normal)
    //    20.0f,           // Frecuencia
    //    true             // Activo desde el inicio
    //);
    
    //m_fountainEmitter = Emissor::createCircleGenerator(
    //    m_particleSystem,
    //    pos,
    //    dir,
    //    pSize,
    //    lifeT,
    //    pColor,
    //    50.0f, 
    //    1.0f, 10.0f,
    //    5.0f, 10.0f,     // Velocidad mín y máx de salida
    //    Emissor::PROYECTILE,
    //    0.0f, 1.0f,      // Media y desviación típica (para distribución Normal)
    //    20.0f,           // Frecuencia
    //    true             // Activo desde el inicio
    //);
    
    //m_fountainEmitter = Emissor::createRectangleGenerator(
    //    m_particleSystem,
    //    pos,
    //    dir,
    //    pSize,
    //    lifeT,
    //    pColor,
    //    50.0f, 
    //    5.0f, 5.0f,
    //    10.0,10.0f,
    //    5.0f, 10.0f,     // Velocidad mín y máx de salida
    //    Emissor::PROYECTILE,
    //    0.0f, 1.0f,      // Media y desviación típica (para distribución Normal)
    //    20.0f,           // Frecuencia
    //    true             // Activo desde el inicio
    //);
    
    //m_fountainEmitter = Emissor::createCubeGenerator(
    //    m_particleSystem,
    //    pos,
    //    dir,
    //    pSize,
    //    lifeT,
    //    pColor,
    //    10.0f, 
    //    5.0f, 10.0f,     // Velocidad mín y máx de salida
    //    Emissor::PROYECTILE,
    //    0.0f, 1.0f,      // Media y desviación típica (para distribución Normal)
    //    100.0f,           // Frecuencia
    //    true             // Activo desde el inicio
    //);
    
     m_fountainEmitter = Emissor::createSphereGenerator(
        m_particleSystem,
        pos,
        dir,
        pSize,
        lifeT,
        pColor,
        10.0f, 
        5.0f, 10.0f,     // Velocidad mín y máx de salida
        Emissor::PROYECTILE,
        0.0f, 1.0f,      // Media y desviación típica (para distribución Normal)
        freq,           // Frecuencia
        true             // Activo desde el inicio
    );


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
