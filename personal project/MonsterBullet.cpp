#include "stdafx.h"
#include "MonsterBullet.h"
#include "Monster.h"
#include "Player.h"
#include "Scene.h"
#include "BulletTable.h"
#include "Scene.h"
#include "SceneDev1.h"

MonsterBullet::MonsterBullet(const std::string textureId, const std::string n) : SpriteGo(textureId, n)
{
}

MonsterBullet::~MonsterBullet()
{
}




void MonsterBullet::Init()
{
	SpriteGo::Init();
	monsterbullet.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/bossBullet.csv"));
	monsterbullet.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/redBullet.csv"));
	monsterbullet.SetTarget(&sprite);
	//monster = new Monster("");
	SetOrigin(Origins::MC);
	attackTimer = attackRate;
}

void MonsterBullet::Release()
{
	SpriteGo::Release();
}

void MonsterBullet::Reset()
{

	monsterbullet.Play("bossBullet");
	SetPosition(0.f, 0.f);
	damage = 1;
	speed = 0.f;
	bulletCount = 1;
	attackRate = 0.f;
	direction = { 0.f , 0.f };
	tick = 3.f;

}

void MonsterBullet::Update(float dt)
{
	sprite.setOrigin(sprite.getGlobalBounds().width * 0.5f, sprite.getGlobalBounds().height * 0.5f);

	SpriteGo::Update(dt);
	monsterbullet.Update(dt);
	//SetPlayer(player->GetPlayer());
		tick -= dt;
		if (bullettype == Types::Bossbullet)
		{
			SetType(Types::Bossbullet);
			Move(dt);
			attackTimer += dt;
			if (attackTimer > attackRate)
			{
				HitPlayer(dt);
				attackTimer = 0.f;
			}
			//std::cout << "스피드" << speed << std::endl;
			//std::cout << "공격력" << damage << std::endl;
			//std::cout << "포지션" << GetPosition().x << GetPosition().y << std::endl;
			tick = 2.f;
		}


}

void MonsterBullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void MonsterBullet::SetDamage(int number)
{
}

void MonsterBullet::SetType(Types t)
{
	const BulletInfo* info = DATATABLE_MGR.Get<BulletTable>(DataTable::Ids::MonsterBullet)->Get(t);

	bullettype = info->bulletType;
	speed = info->speed;
	bulletCount = info->bulletCount;
	damage = info->damage;
	attackRate = info->attackRate;

	//std::cout << (int)t << std::endl;
}

void MonsterBullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir)
{
	sprite.setRotation(Utils::Angle(dir));
	SetPosition(pos);

	direction = dir;
}

void MonsterBullet::Move(float dt)
{
	//벽에 닿으면 사라지게

	position += direction * speed * dt;
	SetPosition(position);
}


void MonsterBullet::HitPlayer(float dt)
{
	//std::cout << this->player << std::endl;
	//몬스터 리스트를 안가져오니 셋플레이어를 해야되나
	if (player != nullptr)
	{
		if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
			player->OnHitted(damage);
		}
	}
}

void MonsterBullet::SetPlayer(Player* player)
{
	this->player = player;
}

float MonsterBullet::GetAttacktimer()
{
	return attackTimer;
}

//float MonsterBullet::GetattackRate() const
//{
//	return attackRate;
//}

