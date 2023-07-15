#include "stdafx.h"
#include "Monster.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Player.h"
#include "SceneDev1.h"
#include "DataTable.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"

Monster::Monster(const std::string n) : SpriteGo("", n)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
	SpriteGo::Init();
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_Idle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monaster-1_Move.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_attack.csv"));

}

void Monster::Release()
{
	SpriteGo::Release();
}

void Monster::Reset()
{
	SpriteGo::Reset();
	hp = maxHp;
	attackTimer = attackRate;
}

void Monster::Update(float dt)
{
	SpriteGo::Update(dt);
	if (player == nullptr)
		return;



}

void Monster::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Monster::SetType(Types t)
{
	const MonsterInfo& info = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get(t);

	//int index = (int)zombieType;
	monsterType = info.monsterType;
	textureId = info.textureId;
	speed = info.speed;
	maxHp = info.maxHP;
	damage = info.damage;
	attackRate = info.attackRate;
}

Monster::Types Monster::GetType() const
{
	return Types();
}

void Monster::SetPlayer(Player* player)
{
}

void Monster::OnHitBullet(int damage)
{
}

void Monster::LookAtPlayer()
{
}

void Monster::FollowPlayer(float dt)
{
}

void Monster::HitPlayer(float dt)
{
}
