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

class SceneDev1 : public Scene
{
public:



protected:
	//ObjectPool<Monster> poolMonsters;
	sf::Vector2f coinDir;
	sf::Vector2f coinPos;
	float speed = 80;
	SpriteGo* tempCoin;


	std::vector<SpriteGo*> coins;
	SpriteGo* coin;
	ObjectPool<Monster> poolMonsters;
	sf::FloatRect tileSize;


	TileMap* tileMap = nullptr;
	TileMap* tileMap2 = nullptr;
	SpriteGo* mapmap;
	SpriteGo* mapmap2;
	sf::Vector2f size;

	Player* player;
	Element* element;
	bool nextScene;
	Monster::Types monsterType;
	Monster::Types monsterType0 = (Monster::Types)0;
	Monster::Types monsterType1 = (Monster::Types)1;

	Monster* monster;
	MonsterBullet* monsterbullet;

	SpriteGo* clear;
	UIButton* clearbutton;

	SpriteGo* nextdoor;
	int monsterCount = 0;
	std::list<Monster*> monsters;

	//플레이어 UI
	UIButton* testbutton1;
	UIButton* testbutton2;
	UIButton* testbutton3;
	SpriteGo* playerHp;
	SpriteGo* playerMaxHp;
	SpriteGo* expbar;
	SpriteGo* maxexpbar;

public:
	SceneDev1();
	virtual ~SceneDev1() override;
	bool bossdie = false;
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

	void PlayerUI();

	void NextScene();
	template<typename T>
	void ClearObjectPool(ObjectPool<T>& pool);
	//void CreateMonster(int count);
	void OnDieMonster(Monster* monster);
	void GetCoin(std::vector<SpriteGo*> coin);
	sf::FloatRect wallBounds;
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