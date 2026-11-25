#pragma once
#include "ForceEmmiter.h"
#include "Vector3D.h"

class SpringForceEmissor:public ForceEmmiter
{
public:
	SpringForceEmissor(double k, double restLenght, Particle* pA, Particle* pB,
		int numSpheres = 8, float sphereRadius = 0.6f, Vector4 color = Vector4(0.8f, 0.8f, 0.2f, 1.0f));
	~SpringForceEmissor();

	virtual void addForce(Particle* p) override;
	void update(double dt) override;

	inline void setK(double k) noexcept { _k = k; }
	inline void setRestLength(double l) noexcept { _resting_lenght = l; }

protected:
	double _k;
	double _resting_lenght;
	Particle* _pA;
	Particle* _pB;

	int _numSpheres;
	float _sphereRadius;
	std::vector<physx::PxTransform> _poses;
	std::vector<RenderItem*> _renderItems;

	virtual bool isEndpointBValid() const;
	virtual Vector3D<float> getEndpointBPos() const { return _pB->getPos(); };

	void updateVisualTransform();
	void clearRenderItems();
};

