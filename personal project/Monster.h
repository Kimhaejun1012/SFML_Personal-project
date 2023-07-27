#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "MonsterBullet.h"

class UIButton;
class Player;
class Scene;
class TileMap;


class Monster : public SpriteGo
{
public:
	enum class Types
	{
		Monster1,
		Hammer,
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

	struct ClipInfoHammer
	{
		std::string move;
		std::string attack;
		std::string idle;
		bool flipX = false;
		sf::Vector2f point;
	};

	enum class BossPattern
	{
		Move,      
		Pattern1,  
		Pattern2,  
		Pattern3   
	};

	static const int TotalTypes = 3;

protected:

	TileMap* tilemap;

	AnimationController monster;
	ObjectPool<MonsterBullet> poolBullets;

	sf::FloatRect monsterfl;

	// 보스 정보
	sf::Vector2f bossMoveDir;
	float bossbulletRate;
	

	sf::Vector2f textmove;
	//보스패턴보스패턴보스패턴보스패턴보스패턴
	BossPattern currentBossPattern;
	bool bossMoving = true;
	bool isPattern = false;
	bool isone = true;
	float bosscooltime = 5.0f;
	float bosspattern = 0.f;
	float bossMoveTimer = bossMoveDuration;
	float bossMoveDuration = 5.0f;
	int randomPattern = 0;
	bool bossscale = true;
	sf::RectangleShape monstersrec;
	int bossHp;
	int bossMaxHp;

	sf::Vector2f mapsizeLT;
	sf::Vector2f mapsizeRB;

	bool bossAlive = true;
	int bosscount;
	bool monsterscale = true;
	sf::Vector2f windowsize;
	Types monsterType;
	sf::Vector2f playerlook;

	sf::Vector2f pattern31BulletPos;
	// 위치 정보
	sf::Vector2f look;
	sf::Vector2f direction;
	sf::Vector2f direction2;
	//몬스터 움직임
	std::vector<ClipInfo> clipInfos;
	ClipInfo currentClipInfo;

	std::vector<ClipInfoBoss> clipInfosboss;
	ClipInfoBoss currentClipInfoboss;

	std::vector<ClipInfoHammer> clipInfosHammer;
	ClipInfoHammer currentClipInfoHammer;
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

	SpriteGo* bossHpbar;
	SpriteGo* bossMaxHpBar;

	float zero = 0.f;
	bool clear = false;
	float pattern3_1ShootTimer = 0.f;
	float pattern3_2ShootTimer = 0.f;
	float pattern3_1ShootInterval = 1.f; // 패턴3-1 총알 발사 간격
	float pattern3_2ShootInterval = 0.5f; // 패턴3-2 총알 발사 간격

	bool isone2 = true;
public:
	ObjectPool<Monster>* pool;
	Monster(const std::string& textureId = "", const std::string& n = "");
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
	void SpawnBullet2(int count, MonsterBullet::Types t);
	void SetFlipX(bool filp);
	bool GetFlipX() const;
	void BossMove(float dt);
	void MonsterDie(bool monsterdie);
	void BossPattern3_1Shoot(float dt);
	void SpawnBullet32(sf::Vector2f pos, int count, MonsterBullet::Types t);
	void BossPattern1(float dt);
	void BossPattern2(float dt);
	void BossPattern3(float dt);
	void BossPattern4(float dt);
	void BossDefault(float dt);
	void GoombaMove(float dt);
	void Pattern3Bullet(MonsterBullet::Types t);
	void BossHpUI();
	void Settile(TileMap* tile);
	void ClearMonsterBulletPool(bool clear);
	void Setmonstersrec(sf::RectangleShape rec);

};

