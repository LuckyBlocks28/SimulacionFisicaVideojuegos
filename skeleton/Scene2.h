#pragma once
#include "Scene.h"
#include <vector>
class Proyectile;
class Scene2 :
    public Scene
{
public:
    explicit Scene2(std::string name) : Scene(std::move(name)), m_proyectiles(), masaTanque(10.f), masaPistola(0.005f), masaCanon(20.f) {}
    void init() override;
    void update(double dt) override;
    void keyPress(unsigned char key, const physx::PxTransform& camera) override;
    void cleanup() override;
private:
    std::vector<Proyectile*> m_proyectiles;
    float masaTanque;
    float masaPistola;
    float masaCanon;
};

