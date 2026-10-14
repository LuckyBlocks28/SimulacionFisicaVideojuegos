#include "Scene3.h"


void Scene3::init()
{
    Vector3D<float> gravity(0.0f, -9.8f, 0.0f);
    Vector3D<float> minBounds(-50.0f, -5.0f, -50.0f);
    Vector3D<float> maxBounds(50.0f, 100.0f, 50.0f);

    m_particleSystem = new ParticleSystem(gravity, minBounds, maxBounds);

    // Configurar propiedades por defecto de partículas generadas
    m_particleSystem->setDefaultParticleProperties(0.3f,Vector4(0.2f, 0.6f, 1.0f, 1.0f), 5.0f);

    // Crear un emisor tipo "Fuente" (geometría circular emitiendo hacia arriba)
    Vector3D<float> pos(0.0f, 0.0f, 0.0f);
    Vector3D<float> dir(0.0f, 1.0f, 0.0f); // Dirección vertical

    m_fountainEmitter = Emissor::createCircleGenerator(
        m_particleSystem,
        pos,
        dir,
        0.5f,            // Profundidad/Altura del cilindro
        0.2f, 1.0f,      // Radio base y radio superior (abocinado)
        8.0f, 15.0f,     // Velocidad mín y máx de salida
        Emissor::PROYECTILE,
        0.0f, 1.5f,      // Media y desviación típica (para distribución Normal)
        30.0f,           // Frecuencia: 30 partículas por segundo
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
    case ' ':
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
        float newFreq = m_fountainEmitter->getFreq() - 5.0f;
        m_fountainEmitter->setFreq(newFreq < 1.0f ? 1.0f : newFreq);
        break;

        // Disparar una ráfaga instantánea/proyectil desde la posición de la cámara
    case 'f':
    case 'F':
    {
        Vector3D<float> camPos(camera.p);
        Vector3D<float> camDir(camera.q.rotate(physx::PxVec3(0, 0, -1)));

        // Creación directa de una partícula puntual desde el ParticleSystem
        m_particleSystem->addParticle(camPos, camDir * 25.0f, 3.0f, 0.8f, Vector4(1.0f, 0.2f, 0.0f, 1.0f));
    }
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
