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
	monsterbullet.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/FireBall.csv"));
	monsterbullet.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Pattern31Bullet.csv"));
	monsterbullet.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Pattern32Bullet.csv"));	
	monsterbullet.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Pattern4Bullet.csv"));
	monsterbullet.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/HammerBullet.csv"));

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

	//monsterbullet.Play("FireBall");
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

		if (bullettype == Types::FireBall)
		{
			if (monsterbullet.GetCurrentClipId() != "FireBall")
			{
				monsterbullet.Play("FireBall");
			}
			Move(dt);
			attackTimer += dt;
			if (attackTimer > attackRate)
			{
				HitPlayer(dt);
				attackTimer = 0.f;
			}
			tick = 2.f;
		}
		if (bullettype == Types::Pattern31)
		{
			if (monsterbullet.GetCurrentClipId() != "Pattern31Bullet")
			{
				monsterbullet.Play("Pattern31Bullet");
			}
			Move31(dt);
			attackTimer += dt;
			if (attackTimer > attackRate)
			{
				HitPlayer(dt);
				attackTimer = 0.f;
			}
			tick = 2.f;
		}
		if (bullettype == Types::Pattern32)
		{
			if (monsterbullet.GetCurrentClipId() != "Pattern32Bullet")
			{
				monsterbullet.Play("Pattern32Bullet");
			}
			Move3(dt);
		
			attackTimer += dt;
			if (attackTimer > attackRate)
			{
				HitPlayer(dt);
				attackTimer = 0.f;
			}
			tick = 2.f;
		}
		if (bullettype == Types::Pattern4)
		{
			if (monsterbullet.GetCurrentClipId() != "Pattern4Bullet")
			{
				monsterbullet.Play("Pattern4Bullet");
			}
			Move(dt);

			attackTimer += dt;
			if (attackTimer > attackRate)
			{
				HitPlayer(dt);
				attackTimer = 0.f;
			}
			tick = 2.f;
		}
		if (bullettype == Types::Hammer)
		{
			if (monsterbullet.GetCurrentClipId() != "HammerBullet")
			{
				monsterbullet.Play("HammerBullet");
			}
			Move(dt);

			attackTimer += dt;
			if (attackTimer > attackRate)
			{
				HitPlayer(dt);
				attackTimer = 0.f;
			}
			tick = 2.f;
		}



}

void MonsterBullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}





void MonsterBullet::SetType(Types t)
{
	const BulletInfo* info = DATATABLE_MGR.Get<BulletTable>(DataTable::Ids::MonsterBullet)->Get(t);

	bullettype = (MonsterBullet::Types)info->bulletType;
	speed = info->speed;
	bulletCount = info->bulletCount;
	damage = info->damage;
	attackRate = info->attackRate;
	//GetType(bullettype);
}



void MonsterBullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir)
{
	sprite.setRotation(Utils::Angle(dir));
	SetPosition(pos);

	direction = dir;
}

//void MonsterBullet::GetType(Types type)
//{
//	bullettype = type;
//}

MonsterBullet::Types MonsterBullet::ReturnType()
{
	return bullettype;
}

void MonsterBullet::Move(float dt)
{


	position += direction * speed * dt;
	SetPosition(position);



	if (!mapsize.contains(GetPosition()))
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
	}
}

void MonsterBullet::Move31(float dt)
{

	playerlook = Utils::Normalize(player->GetPosition() - GetPosition());
	position += playerlook * speed * dt;
	SetPosition(position);



	if (!mapsize.contains(GetPosition()))
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
	}
}

void MonsterBullet::Move3(float dt)
{
	std::cout << tick3 << std::endl;

	tick3 -= dt;
	if(tick3 > 5.0f)
	{
		position += direction * speed * dt;
		SetPosition(position);

	}
	else if(tick3 > 3.0f)
	{
		position += direction * 0.f * dt;
		SetPosition(position);

	}
	else if(tick3 >= -30.f)
	{
		if(isone)
		{
			direction.x = Utils::RandomRange(-1.f, 1.f);
			direction.y = Utils::RandomRange(-1.f, 1.f);
			isone = false;
			
		}

		position += direction * 1500.f * dt;
	}

	SetPosition(position);

	if (!mapsize.contains(GetPosition()))
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
	}
}


void MonsterBullet::HitPlayer(float dt)
{
	if (player != nullptr)
	{
		if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
			player->OnHitted(damage);
		}
	}
	/*else if (!sprite.getGlobalBounds().contains())
		{

		}*/
}

void MonsterBullet::SetPlayer(Player* player)
{
	this->player = player;
}


void MonsterBullet::SetMapSize(sf::FloatRect& mapsize)
{
	this->mapsize = mapsize;
}

void MonsterBullet::Pattern3(float tick)
{
	tick3 = tick;
}