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

void BoxGeometry::containParticle(Particle* p, float restitution)
{
	if (!p || !p->isActive()) return;

	Vector3D<float> up(0.0f, 1.0f, 0.0f);
	bool needsRotation = (up - m_direction).magnitud() > 0.0001f;

	// 1. Transformar posición y velocidad del espacio global al local del cubo
	Vector3D<float> localPos = p->getPos();
	Vector3D<float> localVel = p->getVel();

	if (needsRotation) {
		rotatePoint(localPos, m_direction, up, m_position);
		rotatePoint(localVel, m_direction, up, Vector3D<float>(0.0f, 0.0f, 0.0f));
	}
	localPos = localPos - m_position;

	float halfW = m_Width * 0.5f;
	float halfH = m_Height * 0.5f;
	float halfD = depth * 0.5f;

	float px = localPos.getX(), py = localPos.getY(), pz = localPos.getZ();
	float vx = localVel.getX(), vy = localVel.getY(), vz = localVel.getZ();
	bool collided = false;

	// 2. Comprobar colisión con las 6 caras interiores e invertir velocidad
	if (px > halfW) { px = halfW;  vx = -std::abs(vx) * restitution; collided = true; }
	if (px < -halfW) { px = -halfW; vx = std::abs(vx) * restitution; collided = true; }

	if (py > halfH) { py = halfH;  vy = -std::abs(vy) * restitution; collided = true; }
	if (py < -halfH) { py = -halfH; vy = std::abs(vy) * restitution; collided = true; }

	if (pz > halfD) { pz = halfD;  vz = -std::abs(vz) * restitution; collided = true; }
	if (pz < -halfD) { pz = -halfD; vz = std::abs(vz) * restitution; collided = true; }

	// 3. Devolver posición corregida y velocidad rebotada al espacio global
	if (collided) {
		Vector3D<float> newPos = m_position + Vector3D<float>(px, py, pz);
		Vector3D<float> newVel(vx, vy, vz);

		if (needsRotation) {
			rotatePoint(newPos, up, m_direction, m_position);
			rotatePoint(newVel, up, m_direction, Vector3D<float>(0.0f, 0.0f, 0.0f));
		}

		p->setPosition(newPos);
		p->setVelocity(newVel);
	}
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