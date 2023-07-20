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
		Boss,
		Monster3,
	};

	enum class MonsterAni
	{
		mob,
		boss,

	};
	static const int TotalTypes = 3;

protected:
	MonsterAni Mob;
	AnimationController monster;

	AnimationController Boss;
	float bossMoveDuration;
	float bossMoveTimer;
	sf::Vector2f bossMoveDir;


	Types monsterType;
	sf::Vector2f look;
	sf::Vector2f direction;
	sf::Vector2f direction2;
	float plusespeed = 0;
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
	sf::FloatRect mapsize;
	sf::Vector2f randPos;

	float tick = 0.f;

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
	const std::list<Monster*> GetMonsterList() const;
	void GetMap2(const sf::FloatRect& mapsize);
};

