#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "SceneDev1.h"
#include "DataTableMgr.h"
#include "Scene.h"
Monster::Monster(const std::string n) : SpriteGo("", n)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Monster::Release()
{
	SpriteGo::Release();
}

void Monster::Reset()
{
	SpriteGo::Reset();
}

void Monster::Update(float dt)
{
	SpriteGo::Update(dt);


}

void Monster::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Monster::SetType(Types t)
{
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
