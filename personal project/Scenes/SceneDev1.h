#pragma once
#include "Scene.h"
#include "ObjectPool.h"
#include "Monster.h"

class TileMap;
class Player;
class Element;
class MapStructure;
class SpriteGo;
class UIButton;
class MonsterBullet;
class TextGo;

class SceneDev1 : public Scene
{
public:



protected:
	//ObjectPool<Monster> poolMonsters;
	sf::Vector2f coinDir;
	sf::Vector2f coinPos;
	float speed = 80;
	SpriteGo* tempCoin;

	SpriteGo* bossHpbar;
	SpriteGo* bossMaxHpBar;
	SpriteGo* lose;

	TextGo* leveluptext;
	TextGo* playerhptex;
	std::vector<SpriteGo*> coins;
	SpriteGo* coin;
	ObjectPool<Monster> poolMonsters;
	sf::FloatRect tileSize;
	sf::Vector2f position;
	sf::Vector2f bossPos;

	bool bosspattern2 = false;
	bool isone = true;
	TileMap* tileMap = nullptr;
	SpriteGo* mapmap;
	sf::Vector2f size;

	sf::FloatRect tilesize;

	int bosshp = 0;
	int bossmaxhp = 0;

	int sin;

	int stage = 1;

	Player* player;
	bool nextScene;
	Monster::Types monsterType;
	Monster::Types monsterType0 = (Monster::Types)0;
	Monster::Types monsterType1 = (Monster::Types)1;
	Monster::Types monsterType2 = (Monster::Types)2;

	Monster* monster;
	MonsterBullet* monsterbullet;

	SpriteGo* clear;
	UIButton* clearbutton;

	SpriteGo* nextdoor;
	int monsterCount = 0;
	std::list<Monster*> monsters;

	SpriteGo* lvtextbar;

	SpriteGo* levelback;

	//플레이어 UI
	UIButton* testbutton1;
	UIButton* testbutton2;
	UIButton* testbutton3;

	sf::FloatRect wallBounds;

	SpriteGo* playerHp;
	SpriteGo* playerMaxHp;
	SpriteGo* expbar;
	SpriteGo* maxexpbar;
	SpriteGo* bossicon;

public:
	SceneDev1();
	virtual ~SceneDev1() override;
	bool bossdie = false;
	int bosson = 1;
	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	//void Scene1MonsterSetting(int count);
	void SpawnMonsters(int count, sf::Vector2f center, Monster::Types a);
	void SpawnMonsters2(int count, sf::Vector2f center);
	const std::list<Monster*>* GetMonsterList() const;

	void BossPattern2(bool bosspattern2);
	

	void NextScene();
	template<typename T>
	void ClearObjectPool(ObjectPool<T>& pool);
	//void CreateMonster(int count);
	void OnDieMonster(Monster* monster);
	void GetCoin(std::vector<SpriteGo*> coin);


	void GetBossHp(int hp);
	void GetBossMaxHp(int maxhp);


};

template<typename T>
inline void SceneDev1::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}