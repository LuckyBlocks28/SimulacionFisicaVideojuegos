#pragma once
#include "Scene.h"
#include "ForceSystem.h"
#include "BuoyancyForceEmissor.h"
#include "LiquidDragForceEmissor.h"
#include <vector>
#include <string>
#include <iostream>

class Scene6 : public Scene
{
public:
	Scene6(std::string name) : Scene(std::move(name)) {};
	void init() override;
	void update(double dt) override;
	void keyPress(unsigned char key, const physx::PxTransform& camera) override;
	void cleanup() override;

private:
	ForceSystem* m_ForceSystem = nullptr;
	std::vector<Particle*> m_particles;
	Particle* m_floatingBox = nullptr;
	BuoyancyForceEmissor* m_buoyancyEmitter = nullptr;
	LiquidDragForceEmissor* m_dragEmitter = nullptr;

	void setLiquid(const std::string& name, float density, float k1, Vector4 color);
};