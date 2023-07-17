#include "stdafx.h"
#include "Bullet.h"
#include "Monster.h"
#include "Scene.h"
#include "Player.h"


Bullet::Bullet(const std::string textureId, const std::string n)
	: SpriteGo(textureId, n)
{
}

Bullet::~Bullet()
{
}


void Bullet::Init()
{
	SpriteGo::Init();
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Bullet.csv"));
	animation.SetTarget(&sprite);
	sprite.setScale(0.3f, 0.3f);
	SetOrigin(Origins::MC);
}

void Bullet::Release()
{
	SpriteGo::Release();
}

void Bullet::Reset()
{
	SpriteGo::Reset();
	damage = 100;
	animation.Play("Bullet");
	sprite.setRotation(0.f);
	SetPosition(0.f, 0.f);
	direction = { 0.f , 0.f };
	speed = 0.f;
	range = 2000.f;
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);
	animation.Update(dt);
	Move(dt);
	HitMonster();
}

void Bullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Bullet::Move(float dt)
{
	range -= speed * dt;
	if (range < 0)
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);		// 풀로 회수
		//SetActive(false);
		return;
	}

	position += direction * speed * dt;
	SetPosition(position);

}

void Bullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed)
{
	sprite.setRotation(Utils::Angle(dir));
	SetPosition(pos);

	direction = dir;
	this->speed = speed;
}

void Bullet::HitMonster()
{
	if (monsters != nullptr)
	{
		for (Monster* monster : *monsters)
		{
			if (sprite.getGlobalBounds().intersects(monster->sprite.getGlobalBounds()))
			{
				std::cout << "몬스터 맞음" << std::endl;
				//std::cout << damage << std::endl;
				monster->OnHitBullet(damage);

				SCENE_MGR.GetCurrScene()->RemoveGo(this);
				pool->Return(this);		// 풀로 회수
				//SetActive(false);
				break;
			}
		}
	}
	//std::cout << "몬스터 충돌 안됨" << std::endl;
}

void Bullet::SetDamage(int number)
{
	damage = number;
}

void Bullet::SetMonsterList(const std::list<Monster*>* list)
{
	monsters = list;
}