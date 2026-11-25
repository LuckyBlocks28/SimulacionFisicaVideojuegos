#pragma once
#include "ForceEmmiter.h"
class BuoyancyForceEmissor :
    public ForceEmmiter
{
public:
	BuoyancyForceEmissor(float height, float volume, float waterHeight, float liquidDensity,
		Particle* targetParticle, float waterSizeXZ = 40.0f,
		Vector4 waterColor = Vector4(0.1f, 0.4f, 0.9f, 0.6f));

	virtual ~BuoyancyForceEmissor();

	virtual void addForce(Particle* p) override;

	inline void setVolume(float v) noexcept { _volume = (v > 0.1f) ? v : 0.1f; }
	inline float getVolume() const noexcept { return _volume; }

	inline void setLiquidDensity(float d) noexcept { _liquid_density = d; }
	inline float getLiquidDensity() const noexcept { return _liquid_density; }

	inline void setWaterHeight(float h0) noexcept { _h0 = h0; _waterPose.p.y = h0; }
	inline float getWaterHeight() const noexcept { return _h0; }

	inline void setWaterColor(Vector4 color) noexcept
	{
		if (_waterRenderItem) {
			_waterRenderItem->color = color;
		}
	}

protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _h0;

	Particle* _targetParticle;

	physx::PxTransform _waterPose;
	RenderItem* _waterRenderItem = nullptr;
};

