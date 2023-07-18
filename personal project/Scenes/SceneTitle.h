#pragma once
#include "Scene.h"

class SpriteGo;
class SceneTitle : public Scene
{
protected:

	sf::Color getColor;
	int steps = 100;
	int duration = 100;
	int currentAlpha;
	SpriteGo* title;
	float speed;

	float fadeDuration = 0.009f;
	float fadeTimer = 0.f;

public:
	SceneTitle(SceneId id = SceneId::Title);
	virtual ~SceneTitle() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};