#include "Emissor.h"
#include "ParticleSystem.h"

#include <cmath>
#include <algorithm>

Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float minVel, float maxVel, TYPE type, float mean, float stdD, float freq, bool active, GEOMETRY geometry)
    : m_position(position), m_direction(direction), m_minVel(minVel), m_maxVel(maxVel),m_type(type), m_mean(mean), m_stdDeviation(stdD), m_freq(freq), m_active(active),
    m_geometry(geometry), m_densityFunc(UNIFORM), m_currT(0.0f), m_pS(nullptr)
{
    std::random_device rd;
    m_engine.seed(rd());
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float minVel, float maxVel, TYPE type, DATA::rectangle data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, minVel, maxVel, type, mean, stdD, freq, active, RECTANGLE)
{
    m_data.m_rectangle = data;
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float minVel, float maxVel, TYPE type, DATA::square data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, minVel, maxVel, type, mean, stdD, freq, active, SQUARE)
{
    m_data.m_square = data;
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float minVel, float maxVel, TYPE type, DATA::circle data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, minVel, maxVel, type, mean, stdD, freq, active, CIRCLE)
{
    m_data.m_circle = data;
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float minVel, float maxVel, TYPE type, DATA::cube data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, minVel, maxVel, type, mean, stdD, freq, active, CUBE)
{
    m_data.m_cube = data;
}

Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float minVel, float maxVel, TYPE type, DATA::box data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, minVel, maxVel, type, mean, stdD, freq, active, BOX)
{
    m_data.m_box = data;
}

Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float minVel, float maxVel, TYPE type, DATA::sphere data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, minVel, maxVel, type, mean, stdD, freq, active, SPHERE)
{
    m_data.m_sphere = data;
}

float Emissor::uniformRandom(float min, float max) {
    if (min >= max) return min;
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_engine);
}

float Emissor::normalRandom(float min, float max, float mean, float stdDeviation) {
    std::normal_distribution<float> dist(mean, stdDeviation);
    float val = dist(m_engine);
    // Acotación dentro de los rangos mínimo y máximo esperados
    return (val < min) ? min : ((val > max) ? max : val);
}

float Emissor::newFloat(float min, float max) {
    if (m_densityFunc == UNIFORM) {
        return uniformRandom(min, max);
    }
    else {
        return normalRandom(min, max, m_mean, m_stdDeviation);
    }
}

Vector3D<float> Emissor::newPosition() {
    Vector3D<float> offset(0.0f, 0.0f, 0.0f);
    const float PI = 3.1415926535f;

    switch (m_geometry) {
    case RECTANGLE: {
        float z = uniformRandom(0.0f, m_data.m_rectangle.depth);
        float t = (m_data.m_rectangle.depth > 0.0f) ? (z / m_data.m_rectangle.depth) : 0.0f;

        float currentW = m_data.m_rectangle.w + t * (m_data.m_rectangle.W - m_data.m_rectangle.w);
        float currentH = m_data.m_rectangle.h + t * (m_data.m_rectangle.H - m_data.m_rectangle.h);

        offset.setX(uniformRandom(-currentW * 0.5f, currentW * 0.5f));
        offset.setY(uniformRandom(-currentH * 0.5f, currentH * 0.5f));
        offset.setZ(z);
        break;
    }
    case SQUARE: {
        float z = uniformRandom(0.0f, m_data.m_square.depth);
        float t = (m_data.m_square.depth > 0.0f) ? (z / m_data.m_square.depth) : 0.0f;

        float currentSide = m_data.m_square.l + t * (m_data.m_square.L - m_data.m_square.l);

        offset.setX(uniformRandom(-currentSide * 0.5f, currentSide * 0.5f));
        offset.setY(uniformRandom(-currentSide * 0.5f, currentSide * 0.5f));
        offset.setZ(z);
        break;
    }
    case CIRCLE: {
        float z = uniformRandom(0.0f, m_data.m_circle.depth);
        float t = (m_data.m_circle.depth > 0.0f) ? (z / m_data.m_circle.depth) : 0.0f;

        float currentR = m_data.m_circle.l + t * (m_data.m_circle.L - m_data.m_circle.l);

        // Raíz cuadrada para asegurar distribución uniforme en la superficie del disco
        float r = std::sqrt(uniformRandom(0.0f, 1.0f)) * currentR;
        float theta = uniformRandom(0.0f, 2.0f * PI);

        offset.setX(r * std::cos(theta));
        offset.setY(r * std::sin(theta));
        offset.setZ(z);
        break;
    }
    case CUBE: {
        float halfSize = m_data.m_cube.size * 0.5f;
        offset.setX(uniformRandom(-halfSize, halfSize));
        offset.setY(uniformRandom(-halfSize, halfSize));
        offset.setZ(uniformRandom(-halfSize, halfSize));
        break;
    }
    case BOX: {
        offset.setX(uniformRandom(-m_data.m_box.w * 0.5f, m_data.m_box.w * 0.5f));
        offset.setY(uniformRandom(-m_data.m_box.h * 0.5f, m_data.m_box.h * 0.5f));
        offset.setZ(uniformRandom(-m_data.m_box.depth * 0.5f, m_data.m_box.depth * 0.5f));
        break;
    }
    case SPHERE: {
        float u = uniformRandom(0.0f, 1.0f);
        float v = uniformRandom(0.0f, 1.0f);
        float w = uniformRandom(0.0f, 1.0f);

        float theta = u * 2.0f * PI;
        float phi = std::acos(2.0f * v - 1.0f);
        float r = m_data.m_sphere.radius * std::cbrt(w); // Raíz cúbica para volumen uniforme

        offset.setX(r * std::sin(phi) * std::cos(theta));
        offset.setY(r * std::sin(phi) * std::sin(theta));
        offset.setZ(r * std::cos(phi));
        break;
    }
    }

    return m_position + offset;
}
Vector3D<float> Emissor::newDirection(const Vector3D<float> pos) {
    if (m_densityFunc == UNIFORM) {
        float speed = uniformRandom(m_minVel, m_maxVel);
        Vector3D<float> dir = m_direction;
        dir.normalize();
        return dir * speed;
    }
    else {
        // Distribución Normal / Gaussiana aplicada a componentes o magnitud
        float speed = normalRandom(m_minVel, m_maxVel, m_mean, m_stdDeviation);

        // Dispersión gaussiana en componentes de dirección
        Vector3D<float> baseDir = m_direction;
        baseDir.normalize();

        Vector3D<float> vel(
            baseDir.getX() * speed + normalRandom(-1.0f, 1.0f, 0.0f, m_stdDeviation),
            baseDir.getY() * speed + normalRandom(-1.0f, 1.0f, 0.0f, m_stdDeviation),
            baseDir.getZ() * speed + normalRandom(-1.0f, 1.0f, 0.0f, m_stdDeviation)
        );
        return vel;
    }
}

bool Emissor::update(double t) {
    if (!m_active || m_freq <= 0.0f) return false;

    m_currT += static_cast<float>(t);
    float interval = 1.0f / m_freq;
    bool emitted = false;

    while (m_currT >= interval) {
        Vector3D<float> pPos = newPosition();
        Vector3D<float> pVel = newDirection(pPos);

        if (m_pS != nullptr) {
            // Se pasa m_type (PARTICLE o PROYECTILE)
            m_pS->addParticle(pPos, pVel, m_type, 4.0f);
        }

        m_currT -= interval;
        emitted = true;
    }

    return emitted;
}

Emissor* Emissor::createRectangleGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float depth, float groundWidth, float groundHeight, float airWidth, float airHeight,
    float m_minVel, float m_maxVel, TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::rectangle data;
    data.w = groundWidth;
    data.h = groundHeight;
    data.W = airWidth;
    data.H = airHeight;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createSquareGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float depth, float groundSize, float airSize, float m_minVel, float m_maxVel,
    TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::square data;
    data.l = groundSize;
    data.L = airSize;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createCircleGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float depth, float groundRadius, float airRadius, float m_minVel, float m_maxVel,
    TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::circle data;
    data.l = groundRadius;
    data.L = airRadius;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createCubeGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float size, float m_minVel, float m_maxVel, TYPE particleType,
    float mean, float stdD, float freq, bool active)
{
    DATA::cube data;
    data.size = size;

    Emissor* emitter = new Emissor(position, direction, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createBoxGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float width, float height, float depth, float m_minVel, float m_maxVel,
    TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::box data;
    data.w = width;
    data.h = height;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createSphereGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float radius, float m_minVel, float m_maxVel, TYPE particleType,
    float mean, float stdD, float freq, bool active)
{
    DATA::sphere data;
    data.radius = radius;

    Emissor* emitter = new Emissor(position, direction, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}