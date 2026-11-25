#include "SpringForceEmissor.h"

SpringForceEmissor::SpringForceEmissor(double k, double restLength, Particle* pA, Particle* pB,
	int numSpheres, float sphereRadius, Vector4 color):
	ForceEmmiter(), _k(k),_resting_lenght(restLength), _pA(pA),_pB(pB), _numSpheres(numSpheres), _sphereRadius(sphereRadius)
{
	m_active = true;

	if (_numSpheres > 0) {
		// Reservamos el tamaño definitivo antes de pasar &_poses[i] a RenderItem
		_poses.resize(_numSpheres, physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));
		_renderItems.reserve(_numSpheres);

		for (int i = 0; i < _numSpheres; ++i) {
			physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(_sphereRadius));
			RenderItem* item = new RenderItem(shape, &_poses[i], color);
			_renderItems.push_back(item);
		}
	}

	updateVisualTransform();
}
SpringForceEmissor::~SpringForceEmissor()
{
	clearRenderItems();
}
void SpringForceEmissor::clearRenderItems()
{
	for (auto* item : _renderItems) {
		if (item) {
			item->release();
		}
	}
	_renderItems.clear();
}
void SpringForceEmissor::update(double dt)
{
	updateVisualTransform();
}
bool SpringForceEmissor::isEndpointBValid() const
{
	return _pB != nullptr && _pB->isActive();
}
void SpringForceEmissor::updateVisualTransform()
{
	if (!_pA || !isEndpointBValid() || _renderItems.empty()) return;

	if (!_pA->isActive()) {
		clearRenderItems();
		return;
	}

	physx::PxVec3 posA(_pA->getPos());
	physx::PxVec3 posB(getEndpointBPos());
	physx::PxVec3 diff = posB - posA;

	for (int i = 0; i < _numSpheres; ++i) {
		float t = static_cast<float>(i + 1) / static_cast<float>(_numSpheres + 1);
		_poses[i].p = posA + diff * t;
	}
}
void SpringForceEmissor::addForce(Particle* particle)
{
	if (!_pA || !_pA->isActive() || !isEndpointBValid()) return;

	Vector3D<float> targetPos;

	if (particle == _pA) {
		targetPos = getEndpointBPos();
	}
	else if (_pB && particle == _pB) {
		targetPos = _pA->getPos();
	}
	else {
		return;
	}

	Vector3D<float> relativePosVec = targetPos - particle->getPos();
	const float length = relativePosVec.magnitud();

	if (length <= 0.0001f) return;

	const float delta_x = length - static_cast<float>(_resting_lenght);
	Vector3D<float> direction = relativePosVec / length;
	Vector3D<float> force = direction * (delta_x * static_cast<float>(_k));

	particle->addForce(force);
}
