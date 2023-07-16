#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Player;
class Scene;

class Monster : public SpriteGo
{
public:
	enum class Types
	{
		Monster1,
		Monster2,
		Monster3,
	};
	static const int TotalTypes = 3;

protected:
	AnimationController animation;
	Types monsterType;
	sf::Vector2f look;
	sf::Vector2f direction;

	float speed = 0.f;
	int maxHp = 0;
	int damage = 0;
	int hp = 0;
	float attackRate = 0.f;
	float attackTimer = 0.f;
	int monsterCount = 0;
	float damageTick = 0.f;
	bool isHit = false;
	Scene* scene;
	Player* player = nullptr;

public:
	ObjectPool<Monster>* pool;
	Monster(const std::string n = "");
	virtual ~Monster() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetType(Types t);
	Types GetType() const;

	void SetPlayer(Player* player);

	void OnHitBullet(int damage);

	void LookAtPlayer();
	void FollowPlayer(float dt);
	void HitPlayer(float dt);
	void GetMonsterPos();
	const std::list<Monster*> GetMonsterList() const;
	void AddMonster(Monster& monster);
};

