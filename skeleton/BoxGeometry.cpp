#include "BoxGeometry.h"
#include <algorithm>
#include <cmath>
#include <random>

BoxGeometry::BoxGeometry(Vector3D<float> pos, Vector3D<float> dir, float w, float h, float d)
	: Geometry(pos, dir), m_Width(w), m_Height(h), depth(d)
{
}

Vector3D<float> BoxGeometry::calculateNewPosition(float factor)
{
	float normFactor = std::clamp(factor, 0.0f, 1.0f);

	// Generador aleatorio independiente para los ejes secundario (Y y Z)
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	float u1 = normFactor; // Eje X se rige por la densidad del Emissor (Uniforme / Normal)
	float u2 = dist(rng);   // Eje Y independiente
	float u3 = dist(rng);   // Eje Z independiente

	// Generar coordenadas locales centradas en el origen [-ancho/2, ancho/2]
	x = (u1 - 0.5f) * m_Width;
	y = (u2 - 0.5f) * m_Height;
	z = (u3 - 0.5f) * depth;

	offset = Vector3D<float>(x, y, z);

	// Posición global trasladada y orientada
	Vector3D<float> finalPos = m_position + offset;
	rotatePoint(finalPos, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, m_position);

	return finalPos;
}

Vector3D<float> BoxGeometry::calculateDirection(float factor)
{
	// Dirección radial de dispersión desde el centro del cubo hacia la posición de la partícula
	Vector3D<float> localDir = offset;

	if (localDir.magnitud() < 0.0001f) {
		localDir = Vector3D<float>(0.0f, 1.0f, 0.0f); // Evitar vector nulo en el centro exacto
	}

	// Orientar la dirección hacia el eje m_direction global
	rotatePoint(localDir, Vector3D<float>(0.0f, 1.0f, 0.0f), m_direction, Vector3D<float>(0.0f, 0.0f, 0.0f));
	localDir.normalize();

	return localDir;
}

bool BoxGeometry::inArea(Particle* p)
{
	if (!p || !p->isActive()) return false;

	// Transformar posición global a espacio local
	Vector3D<float> localPos = p->getPos();
	rotatePoint(localPos, m_direction, Vector3D<float>(0.0f, 1.0f, 0.0f), m_position);
	localPos = localPos - m_position;

	float halfW = m_Width * 0.5f;
	float halfH = m_Height * 0.5f;
	float halfD = depth * 0.5f;

	return (std::abs(localPos.getX()) <= halfW) &&
		(std::abs(localPos.getY()) <= halfH) &&
		(std::abs(localPos.getZ()) <= halfD);
}