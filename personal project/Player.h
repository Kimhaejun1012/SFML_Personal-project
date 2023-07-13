#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player : public SpriteGo
{
public:

protected:
	AnimationController animation;
	sf::Vector2f velocity;
	sf::RectangleShape floor;
	float accel = 500.f;
	float speed = 500.f;
	float JumpForce = -250.f;
	float gravity = 500.f;

	bool filpX = false;
	bool isGround = true;

	bool adasd = false;

public:
	Player(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	bool GetFlipX() const;
	void SetFlipX(bool filp);

	virtual void Draw(sf::RenderWindow& window) override;

};
