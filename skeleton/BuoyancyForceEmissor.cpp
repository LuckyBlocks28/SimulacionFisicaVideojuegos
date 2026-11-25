#include "BuoyancyForceEmissor.h"

BuoyancyForceEmissor::BuoyancyForceEmissor(float height, float volume, float waterHeight, float liquidDensity,
	Particle* targetParticle, float waterSizeXZ, Vector4 waterColor)
	: ForceEmmiter(), _height(height), _volume(volume), _liquid_density(liquidDensity),
	_h0(waterHeight), _targetParticle(targetParticle),
	_waterPose(physx::PxTransform(physx::PxVec3(0.0f, waterHeight, 0.0f)))
{
	m_active = true;

	// Cubo con componente vertical muy pequeña (0.05f) que simula la superficie del agua
	physx::PxShape* waterShape = CreateShape(physx::PxBoxGeometry(waterSizeXZ, 0.05f, waterSizeXZ));
	_waterRenderItem = new RenderItem(waterShape, &_waterPose, waterColor);
}

BuoyancyForceEmissor::~BuoyancyForceEmissor()
{
	if (_waterRenderItem) {
		_waterRenderItem->release();
		_waterRenderItem = nullptr;
	}
}

void BuoyancyForceEmissor::addForce(Particle* p)
{
	// Aplicar el empuje solo a la partícula designada como objeto flotante
	if (!p || p != _targetParticle || !p->isActive()) return;

	const float h = p->getPos().getY();
	float immersed = 0.0f;

	if (h - _h0 > _height * 0.5f) {
		// Caso (a): Completamente fuera del agua
		immersed = 0.0f;
	}
	else if (_h0 - h > _height * 0.5f) {
		// Caso (b): Completamente sumergido
		immersed = 1.0f;
	}
	else {
		// Caso (c): Parcialmente sumergido
		immersed = (_h0 - h) / _height + 0.5f;
	}

	// Fuerza de empuje vertical hacia arriba: E = rho * V * immersed * g
	Vector3D<float> f(0.0f, 0.0f, 0.0f);
	f.setY(_liquid_density * _volume * immersed * 9.8f);

	p->addForce(f);
}
