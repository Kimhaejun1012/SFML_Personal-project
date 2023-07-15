#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player : public SpriteGo
{
public:

protected:
	AnimationController animation;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	sf::RectangleShape floor;

	sf::FloatRect playerwall;

	float accel = 500.f;
	float speed = 500.f;

	bool filpX = false;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

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
	void Getvelocity();
	void SetWallBounds(const sf::FloatRect& bounds);
};
