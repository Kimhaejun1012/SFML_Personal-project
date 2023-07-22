#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "MonsterBullet.h"

class UIButton;
class Player;
class Scene;

class Monster : public SpriteGo
{
public:
	enum class Types
	{
		Monster1,
		Monster2,
		Boss,
	};

	struct ClipInfo
	{
		std::string move;
		std::string run;
		//std::string attack;
		bool flipX = false;
		sf::Vector2f point;
	};
	struct ClipInfoBoss
	{
		std::string move;
		std::string run;
		//std::string attack;
		bool flipX = false;
		sf::Vector2f point;
	};
	static const int TotalTypes = 3;

protected:

	AnimationController monster;
	ObjectPool<MonsterBullet> poolBullets;

	// 보스 정보
	float bossMoveDuration;
	float bossMoveTimer;
	sf::Vector2f bossMoveDir;
	float bossbulletRate;

	bool bossAlive = true;
	int bosscount;
	bool bossMoving = true;
	bool monsterscale = true;
	sf::Vector2f windowsize;
	Types monsterType;
	sf::Vector2f playerlook;

	// 위치 정보
	sf::Vector2f look;
	sf::Vector2f direction;
	sf::Vector2f direction2;

	//몬스터 움직임
	std::vector<ClipInfo> clipInfos;
	ClipInfo currentClipInfo;

	std::vector<ClipInfoBoss> clipInfosboss;
	ClipInfoBoss currentClipInfoboss;
	bool flipX = false;
	// 스탯
	//float plusespeed = 0;
	float speed = 0.f;
	int maxHp = 0;
	int damage = 0;
	int hp = 0;

	float attackRate = 0.f;
	float attackTimer = 0.f;
	int monsterCount = 0;
	float damageTick = 0.f;
	float bosspattern = 10.f;
	// 상태변수
	bool isHit = false;

	Scene* scene;
	Player* player = nullptr;
	sf::FloatRect mapsize;
	sf::Vector2f randPos;

	bool monsterdie = false;
	MonsterBullet* monsterbullet = nullptr;
	std::list<MonsterBullet*> monsterbullets;
	float tick = 5.f;

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

	void HitMonsterBullet(int damage);
	void OnHitBullet(int damage);
	void LookAtPlayer();
	void FollowPlayer(float dt);
	void HitPlayer(float dt);
	const std::list<Monster*> GetMonsterList() const;
	void GetMap2(const sf::FloatRect& mapsize);
	void Shoot();
	void SpawnBullet(MonsterBullet::Types t);
	void SetFlipX(bool filp);
	bool GetFlipX() const;
	void BossMove(float dt);
	void MonsterDie(bool monsterdie);
};

