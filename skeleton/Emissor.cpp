#include "Emissor.h"
#include "ParticleSystem.h"

Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float size, float lifeTime, Vector4 color, float minVel, float maxVel, TYPE type, float mean, float stdD, float freq, bool active, GEOMETRY geometry)
    : m_position(position), m_direction(direction),m_size(size), m_minVel(minVel), m_maxVel(maxVel),m_type(type), m_mean(mean), m_stdDeviation(stdD), m_freq(freq), m_active(active),
    m_geometry(geometry), m_densityFunc(UNIFORM), m_currT(0.0f), m_pS(nullptr),m_lifeTime(lifeTime),m_color(color)
{
    std::random_device rd;
    m_engine.seed(rd());
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float size, float lifeTime, Vector4 color, float minVel, float maxVel, TYPE type, 
    DATA::rectangle data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction,size,lifeTime,color, minVel, maxVel, type, mean, stdD, freq, active, RECTANGLE)
{
    m_data.m_rectangle = data;
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float size, float lifeTime, Vector4 color, float minVel, float maxVel, TYPE type, 
    DATA::square data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, size, lifeTime, color, minVel, maxVel, type, mean, stdD, freq, active, SQUARE)
{
    m_data.m_square = data;
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float size, float lifeTime, Vector4 color, float minVel, float maxVel, TYPE type, 
    DATA::circle data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, size, lifeTime, color, minVel, maxVel, type, mean, stdD, freq, active, CIRCLE)
{
    m_data.m_circle = data;
}
Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float size, float lifeTime, Vector4 color, float minVel, float maxVel, TYPE type, 
    DATA::cube data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, size, lifeTime, color, minVel, maxVel, type, mean, stdD, freq, active, CUBE)
{
    m_data.m_cube = data;
}

Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float size, float lifeTime, Vector4 color, float minVel, float maxVel, TYPE type,
    DATA::box data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, size, lifeTime, color, minVel, maxVel, type, mean, stdD, freq, active, BOX)
{
    m_data.m_box = data;
}

Emissor::Emissor(Vector3D<float> position, Vector3D<float> direction, float size, float lifeTime, Vector4 color, float minVel, float maxVel, TYPE type,
    DATA::sphere data, float mean, float stdD, float freq, bool active)
    : Emissor(position, direction, size, lifeTime, color, minVel, maxVel, type, mean, stdD, freq, active, SPHERE)
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
Vector3D<float> Emissor::calculateDirection()
{
    Vector3D<float> originPos(0.0f, 0.0f, 0.0f);
    Vector3D<float> topPos(0.0f, 0.0f, 0.0f);
    Vector3D<float> newDirection(0.0f, -1.0f, 0.0f);
    constexpr float TWO_PI = 2.0f * std::numbers::pi_v<float>;
    float randomDistr = newFloat(-1, 1);
    float x, y, z, r, halfSize;
    float theta,cosTheta, sinTheta;

    switch (m_geometry) 
    {
    case Emissor::RECTANGLE:
        x = (m_data.m_rectangle.w * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_rectangle.h * 0.5f) * newFloat(-1.0f, 1.0f);
        originPos = Vector3D<float>(x, 0.0f, z);

        x = (m_data.m_rectangle.W * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_rectangle.H * 0.5f) * newFloat(-1.0f, 1.0f);
        topPos = Vector3D<float>(x, m_data.m_rectangle.depth, z);

        newDirection = topPos - originPos;
        break;
    case Emissor::SQUARE:
        x = (m_data.m_square.l * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_square.l * 0.5f) * newFloat(-1.0f, 1.0f);
        originPos = Vector3D<float>(x, 0.0f, z);

        x = (m_data.m_square.L * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_square.L * 0.5f) * newFloat(-1.0f, 1.0f);
        topPos = Vector3D<float>(x, m_data.m_square.depth, z);

        newDirection = topPos - originPos;
        break;
    case Emissor::CIRCLE:
        theta = newFloat(0.0f, TWO_PI);
        r = m_data.m_circle.l * std::sqrt(uniformRandom(0.0f, 1.0f));
        originPos = Vector3D<float>(r * std::cos(theta), 0.0f, r * std::sin(theta));

        r = m_data.m_circle.L * std::sqrt(uniformRandom(0.0f, 1.0f));
        topPos = Vector3D<float>(r * std::cos(theta), m_data.m_circle.depth, r * std::sin(theta));

        newDirection = topPos - originPos;
        break;
    case Emissor::CUBE:
    case Emissor::BOX:
    case Emissor::SPHERE:
        halfSize = m_data.m_cube.size * 0.5f;
        x = newFloat(-1.0, 1.0);
        y = newFloat(-1.0, 1.0);
        z = newFloat(-1.0, 1.0);
        newDirection = Vector3D<float>(x, y, z);
        break;
    default:
        break;
    }
    rotatePoint(newDirection, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, Vector3D<float>(0.0f, 0.0f, 0.0f));
    newDirection.normalize();
    return newDirection;
}
Vector3D<float> Emissor::calculateNewPosition()
{
    Vector3D<float> offset(0.0f, 0.0f, 0.0f);
    constexpr float TWO_PI = 2.0f * std::numbers::pi_v<float>;

    float x = 0.0f, y = 0.0f, z = 0.0f;
    float r = 0.0f, theta = 0.0f;

    switch (m_geometry)
    {
    case Emissor::RECTANGLE:
        // Superficie 2D en plano XZ basada en ancho (w) y alto (h) del plano base
        x = (m_data.m_rectangle.w * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_rectangle.h * 0.5f) * newFloat(-1.0f, 1.0f);
        offset = Vector3D<float>(x, 0.0f, z);
        break;

    case Emissor::SQUARE:
        // Superficie 2D en plano XZ basada en lado (l)
        x = (m_data.m_square.l * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_square.l * 0.5f) * newFloat(-1.0f, 1.0f);
        offset = Vector3D<float>(x, 0.0f, z);
        break;

    case Emissor::CIRCLE:
        // Disco 2D en plano XZ con distribución uniforme dentro del radio de la base (l)
        theta = uniformRandom(0.0f, TWO_PI);
        r = m_data.m_circle.l * std::sqrt(uniformRandom(0.0f, 1.0f));
        x = r * std::cos(theta);
        z = r * std::sin(theta);
        offset = Vector3D<float>(x, 0.0f, z);
        break;

    case Emissor::CUBE:
        // Volumen 3D centrado
        x = (m_data.m_cube.size * 0.5f) * newFloat(-1.0f, 1.0f);
        y = (m_data.m_cube.size * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_cube.size * 0.5f) * newFloat(-1.0f, 1.0f);
        offset = Vector3D<float>(x, y, z);
        break;

    case Emissor::BOX:
        // Volumen 3D con dimensiones w, h, depth
        x = (m_data.m_box.w * 0.5f) * newFloat(-1.0f, 1.0f);
        y = (m_data.m_box.h * 0.5f) * newFloat(-1.0f, 1.0f);
        z = (m_data.m_box.depth * 0.5f) * newFloat(-1.0f, 1.0f);
        offset = Vector3D<float>(x, y, z);
        break;

    case Emissor::SPHERE:
        // Volumen 3D con distribución uniforme dentro de la esfera
    {
        float u = uniformRandom(0.0f, 1.0f);
        r = m_data.m_sphere.radius * std::cbrt(u);
        float cosTheta = newFloat(-1.0f, 1.0f);
        float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
        float phi = uniformRandom(0.0f, TWO_PI);

        x = r * sinTheta * std::cos(phi);
        y = r * sinTheta * std::sin(phi);
        z = r * cosTheta;
        offset = Vector3D<float>(x, y, z);
    }
    break;

    default:
        break;
    }
    Vector3D<float> finalPos = m_position + offset;
    rotatePoint(finalPos, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, m_position);
    return finalPos;
}
void Emissor::rotatePoint(Vector3D<float>& position, const Vector3D<float>& u, const Vector3D<float>& v, const Vector3D<float>& center)
{
    const Vector3D<float> uX = { 0.f, u.getY(), u.getZ() };
    const Vector3D<float> vX = { 0.f, v.getY(), v.getZ() };
    const float magX = uX.magnitud() * vX.magnitud();
    float angX = magX > 0.f ? std::acosf(uX.dot(vX) / magX) : 0.f;
    if ((uX.getY() * vX.getZ() - uX.getZ() * vX.getY()) < 0.f) angX = -angX;

    const Vector3D<float> uY = { u.getX(), 0.f, u.getZ() };
    const Vector3D<float> vY = { v.getX(), 0.f, v.getZ() };
    const float magY = uY.magnitud() * vY.magnitud();
    float angY = magY > 0.f ? std::acosf(uY.dot(vY) / magY) : 0.f;
    if ((uY.getZ() * vY.getX() - uY.getX() * vY.getZ()) < 0.f) angY = -angY;

    const Vector3D<float> uZ = { u.getX(), u.getY(), 0.f };
    const Vector3D<float> vZ = { v.getX(), v.getY(), 0.f };
    const float magZ = uZ.magnitud() * vZ.magnitud();
    float angZ = magZ > 0.f ? std::acosf(uZ.dot(vZ) / magZ) : 0.f;
    if ((uZ.getX() * vY.getY() - uZ.getY() * vZ.getX()) < 0.f) angZ = -angZ;

    const float cx = cosf(angX), sx = sinf(angX);
    const float cy = cosf(angY), sy = sinf(angY);
    const float cz = cosf(angZ), sz = sinf(angZ);

    position.setX(position.getX() - center.getX());
    position.setY(position.getY() - center.getY());
    position.setZ(position.getZ() - center.getZ());

    //Rotación en X
    const float y1 = position.getY() * cx - position.getZ() * sx;
    const float z1 = position.getY() * sx + position.getZ() * cx;
    position.setY(y1);
    position.setZ(z1);

    // Rotación en Y
    const float x2 = position.getX() * cy + position.getZ() * sy;
    const float z2 = -position.getX() * sy + position.getZ() * cy;
    position.setX(x2);
    position.setZ(z2);

    // Rotación en Z
    const float x3 = position.getX() * cz - position.getY() * sz;
    const float y3 = position.getX() * sz + position.getY() * cz;
    position.setX(x3);
    position.setY(y3);

    //DEVOLVER A COORDENADAS GLOBALES
    position.setX(position.getX() + center.getX());
    position.setY(position.getY() + center.getY());
    position.setZ(position.getZ() + center.getZ());
}
bool Emissor::update(double t) {
    if (!m_active || m_freq <= 0.0f) return false;

    m_currT += static_cast<float>(t);
    float interval = 1.0f / m_freq;
    bool emitted = false;

    while (m_currT >= interval) {
        
        if (m_pS != nullptr) {
            // Se pasa m_type (PARTICLE o PROYECTILE)
            m_pS->addParticle(calculateNewPosition(), calculateDirection() * m_maxVel,m_size, m_type,m_lifeTime, m_color);
        }

        m_currT -= interval;
        emitted = true;
    }

    return emitted;
}

Emissor* Emissor::createRectangleGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float pSize, float lifeTime, Vector4 color,
    float depth, float groundWidth, float groundHeight, float airWidth, float airHeight,
    float m_minVel, float m_maxVel, TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::rectangle data;
    data.w = groundWidth;
    data.h = groundHeight;
    data.W = airWidth;
    data.H = airHeight;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, pSize, lifeTime, color, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createSquareGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float pSize, float lifeTime, Vector4 color, 
    float depth, float groundSize, float airSize, float m_minVel, float m_maxVel, TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::square data;
    data.l = groundSize;
    data.L = airSize;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, pSize, lifeTime, color, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createCircleGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float pSize, float lifeTime, Vector4 color, 
    float depth, float groundRadius, float airRadius, float m_minVel, float m_maxVel, TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::circle data;
    data.l = groundRadius;
    data.L = airRadius;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, pSize, lifeTime, color, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createCubeGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float pSize, float lifeTime, Vector4 color, 
    float size, float m_minVel, float m_maxVel, TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::cube data;
    data.size = size;

    Emissor* emitter = new Emissor(position, direction, pSize, lifeTime, color, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createBoxGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float pSize, float lifeTime, Vector4 color, 
    float width, float height, float depth, float m_minVel, float m_maxVel, TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::box data;
    data.w = width;
    data.h = height;
    data.depth = depth;

    Emissor* emitter = new Emissor(position, direction, pSize, lifeTime, color, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}

Emissor* Emissor::createSphereGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float pSize, float lifeTime, Vector4 color,
    float radius, float m_minVel, float m_maxVel, TYPE particleType, float mean, float stdD, float freq, bool active)
{
    DATA::sphere data;
    data.radius = radius;

    Emissor* emitter = new Emissor(position, direction, pSize, lifeTime, color, m_minVel, m_maxVel, particleType, data, mean, stdD, freq, active);
    emitter->m_pS = pS;
    return emitter;
}