#include "CilinderGeometry.h"

CilinderGeometry::CilinderGeometry(Vector3D<float> pos, Vector3D<float> dir, float l, float d, float L)
    : Geometry(pos, dir), m_lenght(l), m_lenght_U(L), depth(d), theta(0.0f), r(0.0f)
{
}

Vector3D<float> CilinderGeometry::calculateNewPosition(float factor)
{
    float normFactor = std::clamp(factor, 0.0f, 1.0f);

    // Generar un ángulo theta uniforme independiente (360°)
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> distTheta(0.0f, TWO_PI);

    theta = distTheta(rng);
    r = m_lenght * std::sqrt(normFactor); // Radio en la base inferior

    // Coordenadas locales en el plano de origen (inferior)
    x = r * std::cos(theta);
    z = r * std::sin(theta);
    y = 0.0f;

    offset = Vector3D<float>(x, y, z);

    // Posición global trasladada y rotada
    Vector3D<float> finalPos = m_position + offset;
    rotatePoint(finalPos, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, m_position);

    return finalPos;
}

Vector3D<float> CilinderGeometry::calculateDirection(float factor)
{
    // Mapeo proporcional del radio hacia el plano superior
    float scale = (m_lenght > 0.0001f) ? (m_lenght_U / m_lenght) : 1.0f;

    // Punto correspondiente en el plano superior
    float xTop = x * scale;
    float zTop = z * scale;
    float yTop = depth;

    // Vector dirección en espacio local desde la posición de la partícula hacia la tapa superior
    Vector3D<float> localDir(xTop - x, yTop - y, zTop - z);

    if (localDir.magnitud() < 0.0001f) {
        localDir = Vector3D<float>(0.0f, 1.0f, 0.0f);
    }

    // Rotar la dirección según m_direction y normalizar
    rotatePoint(localDir, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, Vector3D<float>(0.0f, 0.0f, 0.0f));
    localDir.normalize();

    return localDir;
}

bool CilinderGeometry::inArea(Particle* p)
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

    // 2. Interpolación lineal del radio a la altura localY
    float t = (depth > 0.0001f) ? (localY / depth) : 0.0f;
    float currentRadius = m_lenght + t * (m_lenght_U - m_lenght);

    // 3. Comprobar la distancia al eje central local (distancia en XZ al cuadrado)
    float distXZSq = (localPos.getX() * localPos.getX()) + (localPos.getZ() * localPos.getZ());

    return distXZSq <= (currentRadius * currentRadius);
}