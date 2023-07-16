#include "stdafx.h"
#include "Monster.h"
#include "Scene.h"
#include "Player.h"
#include "SceneDev1.h"
#include "DataTable.h"
#include "MonsterTable.h"
#include "Bullet.h"

Monster::Monster(const std::string n)
	: SpriteGo("", n)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
	SpriteGo::Init();
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_Idle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_Move.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_Attack.csv"));
	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);

	SetPosition(0, 0);
	
}

void Monster::Release()
{
	SpriteGo::Release();
}

void Monster::Reset()
{
	SpriteGo::Reset();
	animation.Play("monster1Move");
	SetOrigin(origin);
	SetPosition({ 0, 0 });
	hp = maxHp;
	attackTimer = attackRate;
}

void Monster::Update(float dt)
{
	animation.Update(dt);
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
	const MonsterInfo* info = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get(t);

	//int index = (int)zombieType;
	//textureId = info.textureId;
	//monsterType = info.monsterType;
	int index = (int)monsterType;
	speed = info->speed;
	maxHp = info->maxHP;
	damage = info->damage;
	attackRate = info->attackRate;
}

Monster::Types Monster::GetType() const
{
	return monsterType;
}

void Monster::SetPlayer(Player* player)
{
	this->player = player;
}

void Monster::OnHitBullet(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		
		if (scene != nullptr)
		{
			sceneDev1->OnDieMonster(this);
		}
	}
}


void Monster::FollowPlayer(float dt)
{
	float distance = Utils::Distance(player->GetPosition(), position);
	if (distance > 25.f && isActive)
	{
		position += direction * speed * dt;
		SetPosition(position);
	}
}

void Monster::HitPlayer(float dt)
{
}

//const std::list<Monster*>* Monster::GetMonsterList() const
//{
//	return &poolMonsters.GetUseList();
//}