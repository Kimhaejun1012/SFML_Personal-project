#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Monster;
class Player;

class Bullet : public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f attack = { 0.f,0.f };
	float speed = 0.f;
	float range = 2000.f;
	int damage = 100;
	//Player* player;
	Monster* monster;
	const std::list<Monster*>* monsters;
	AnimationController animation;

public:
	ObjectPool<Bullet>* pool;

	Bullet(const std::string textureId = "", const std::string n = "");
	virtual ~Bullet() override;

	void SetMonsterList(const std::list<Monster*>* list);

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void Move(float dt);
	void HitMonster();

	void SetDamage(int number);
	int GetDamage()
	{
		return damage;
	}
};

