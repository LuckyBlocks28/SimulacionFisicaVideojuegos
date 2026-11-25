#pragma once
#include "ForceEmmiter.h"

class LiquidDragForceEmissor : public ForceEmmiter
{
public:
	// k1: coeficiente lineal (dominante en líquidos viscosos)
	// k2: coeficiente cuadrático (0.0f para líquidos, dominante en gases)
	LiquidDragForceEmissor(float k1, float k2, float objectHeight, float waterHeight, Particle* targetParticle);
	virtual ~LiquidDragForceEmissor() = default;

	virtual void addForce(Particle* p) override;

	inline void setCoefficients(float k1, float k2 = 0.0f) noexcept { _k1 = k1; _k2 = k2; }
	inline float getK1() const noexcept { return _k1; }
	inline float getK2() const noexcept { return _k2; }
	inline void setWaterHeight(float h0) noexcept { _h0 = h0; }

protected:
	float _k1;
	float _k2;
	float _height;
	float _h0;
	Particle* _targetParticle;
};