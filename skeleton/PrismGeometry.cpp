#include "PrismGeometry.h"

PrismGeometry::PrismGeometry(Vector3D<float> pos, Vector3D<float> dir, float w, float h, float d, float W, float H):Geometry(pos,dir),m_Width(w),m_Height(h),depth(d),m_Width_U(W),m_Height_U(H)
{
}

Vector3D<float> PrismGeometry::calculateDirection(float factor)
{
    float normFactor = std::clamp(factor, 0.0f, 1.0f);
    float u1 = normFactor;
    float u2 = std::fmod(normFactor * 13.3714f + 0.31415f, 1.0f);

    float xOrig = (u1 - 0.5f) * m_Width;
    float zOrig = (u2 - 0.5f) * m_Height;
    Vector3D<float> originPos(xOrig, 0.0f, zOrig);

    float xTop = (u1 - 0.5f) * m_Width_U;
    float zTop = (u2 - 0.5f) * m_Height_U;
    Vector3D<float> topPos(xTop, depth, zTop);

    Vector3D<float> newDirection = topPos - originPos;
    if (newDirection.magnitud() < 0.0001f) {
        newDirection = Vector3D<float>(0.0f, 1.0f, 0.0f);
    }

    rotatePoint(newDirection, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, Vector3D<float>(0.0f, 0.0f, 0.0f));
    newDirection.normalize();

    return newDirection;
}

Vector3D<float> PrismGeometry::calculateNewPosition(float factor)
{
    float normFactor = std::clamp(factor, 0.0f, 1.0f);
    float u1 = normFactor;
    float u2 = std::fmod(normFactor * 13.3714f + 0.31415f, 1.0f);

    x = (u1 - 0.5f) * m_Width;
    y = 0.0f;
    z = (u2 - 0.5f) * m_Height;

    offset = Vector3D<float>(x, y, z);
    Vector3D<float> finalPos = m_position + offset;

    rotatePoint(finalPos, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, m_position);
    return finalPos;
}

bool PrismGeometry::inArea(Particle* p)
{
    if (!p || !p->isActive()) return false;

    // Transformar posición global a espacio local
    Vector3D<float> localPos = p->getPos();
    rotatePoint(localPos, m_direction, Vector3D<float>(0.0f, 1.0f, 0.0f), m_position);
    localPos = localPos - m_position;

    float localY = localPos.getY();

    // 1. Verificar límites en altura (eje Y local)
    if (localY < 0.0f || localY > depth) {
        return false;
    }

    // 2. Interpolación lineal de ancho y alto a la altura localY
    float t = (depth > 0.0001f) ? (localY / depth) : 0.0f;
    float currentHalfW = 0.5f * (m_Width + t * (m_Width_U - m_Width));
    float currentHalfH = 0.5f * (m_Height + t * (m_Height_U - m_Height));

    // 3. Comprobar si las coordenadas X y Z están dentro de los márgenes
    return (std::abs(localPos.getX()) <= currentHalfW) &&
        (std::abs(localPos.getZ()) <= currentHalfH);
}
