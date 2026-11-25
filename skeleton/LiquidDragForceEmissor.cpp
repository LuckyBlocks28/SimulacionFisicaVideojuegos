#include "LiquidDragForceEmissor.h"

LiquidDragForceEmissor::LiquidDragForceEmissor(float k1, float k2, float objectHeight, float waterHeight, Particle* targetParticle)
	: ForceEmmiter(), _k1(k1), _k2(k2), _height(objectHeight), _h0(waterHeight), _targetParticle(targetParticle)
{
	m_active = true;
}

void LiquidDragForceEmissor::addForce(Particle* p)
{
	if (!p || p != _targetParticle || !p->isActive()) return;

	// 1. Comprobar qué proporción del objeto está dentro del líquido
	const float h = p->getPos().getY();
	float immersed = 0.0f;

	if (h - _h0 > _height * 0.5f) {
		return; // Completamente fuera del líquido: no hay rozamiento viscoso
	}
	else if (_h0 - h > _height * 0.5f) {
		immersed = 1.0f; // Completamente sumergido
	}
	else {
		immersed = (_h0 - h) / _height + 0.5f; // Parcialmente sumergido
	}

	// 2. Calcular fuerza de arrastre: F_drag = -(k1 * |v| + k2 * |v|^2) * v_dir
	Vector3D<float> vel = p->getVel();
	const float speed = vel.magnitud();

	if (speed <= 0.0001f) return;

	// Escalamos los coeficientes según la fracción sumergida
	float dragCoeff = (_k1 * speed + _k2 * speed * speed) * immersed;
	Vector3D<float> dragForce = (vel / -speed) * dragCoeff;

	p->addForce(dragForce);
}