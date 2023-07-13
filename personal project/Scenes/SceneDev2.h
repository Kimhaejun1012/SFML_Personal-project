#pragma once
#include "Scene.h"
class SceneDev2 : public Scene
{
protected:

public:
	SceneDev2();
	virtual ~SceneDev2() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

