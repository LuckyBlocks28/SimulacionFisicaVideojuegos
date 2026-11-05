#include "Emissor.h"
#include "ParticleSystem.h"

Emissor::Emissor(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, Geometry* g, float size, float lifeTime, Vector4 color, float m_minVel, float m_maxVel, TYPE type, float mean, float stdD, float freq, bool active, DENSITYFUNTION func)
    : m_pS(pS), m_position(position), m_direction(direction), m_size(size), m_minVel(m_minVel), m_maxVel(m_maxVel), m_type(type), m_mean(mean), m_stdDeviation(stdD), m_freq(freq), m_active(active),
    m_geom(g), m_densityFunc(func), m_currT(0.0f), m_lifeTime(lifeTime), m_color(color)
{
    std::random_device rd;
    m_engine.seed(rd());
}

Emissor::~Emissor()
{
    delete m_geom;
}
float Emissor::uniformRandom(float min, float max) {
    if (min >= max) return min;
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_engine);
}

float Emissor::normalRandom(float min, float max, float mean, float stdDeviation) {
   std::normal_distribution<float> dist(mean, stdDeviation);
    float val;
    // Descarta valores fuera de rango en lugar de aplastarlos a 0.0f
    do {
        val = dist(m_engine);
    } while (val < min || val > max);
    return val;
}

float Emissor::newFloat(float min, float max) {
    if (m_densityFunc == UNIFORM) {
        return uniformRandom(min, max);
    }
    else {
        return normalRandom(min, max, m_mean, m_stdDeviation);
    }
}
bool Emissor::update(double t) {
    if (m_freq <= 0.0f) return false;

    m_currT += static_cast<float>(t);
    float interval = 1.0f / m_freq;
    bool emitted = false;

    while (m_currT >= interval && m_active) {
        if (m_pS != nullptr) {
            // Factor para el radio (se aplica la densidad Gaussiana o Uniforme)
            float fRadius = newFloat(0.0f, 1.0f);

            Vector3D<float> pos = m_geom->calculateNewPosition(fRadius);
            Vector3D<float> dir = m_geom->calculateDirection(fRadius);

            float speed = uniformRandom(m_minVel, m_maxVel);
            Vector3D<float> vel = dir * speed;

            Vector4 color = Vector4({ newFloat(0.0f, 1.0f), newFloat(0.0f, 1.0f), newFloat(0.0f, 1.0f), 1.0f });

            m_pS->addParticle(pos, vel, 1.0f, m_size, m_lifeTime, m_type, color);
        }

        m_currT -= interval;
        emitted = true;
    }

    return emitted;
}

