#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle();
	Particle(Vector3D<float> p, Vector3D<float> v, float size, Vector4 color = Vector4(0.0, 1.0, 0.0, 1.0));

	virtual ~Particle();
	void integrateEuler(float t);
	void integrateEulerSemi(float t);
	void integrateVerlet(float t);
	void setOriginalPosition();
	void setDamping(float d);

	inline Vector3D<float> getPos() const { return pose.p; }

	inline void setPosition(Vector3D<float> p) noexcept { ogPos = p; pose = physx::PxTransform(p); };
	inline void setVelocity(Vector3D<float> v) noexcept { vel = v; };
	inline void setLifeTime(float lifetime) noexcept { remainingLife = lifetime; };
	inline void setAcceleration(Vector3D<float> a) noexcept { acc = a; };
	void createRenderItem(float size, Vector4 color);
	inline void setActive(bool state) noexcept { active = state; }

	virtual void update(float dt);
	inline bool isActive() const { return active; }
	void deactivate();

protected:
	Vector3D<float> vel;
	Vector3D<float> acc = { 0.0f, 9.8f, 0.0f };
	float damping;

	physx::PxTransform pose;
	physx::PxTransform antPos;
	RenderItem* renderItem;

	Vector3D<float> ogPos;
	bool firstMove;

	bool active = false;
	float remainingLife = 0.0f;

};

#pragma once
