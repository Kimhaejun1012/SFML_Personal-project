#include "stdafx.h"
#include "Player2.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"

void Player2::Init()
{
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "tables/Idle2.csv");
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "tables/Move2.csv");
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "tables/Jump2.csv");



	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Idle2.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Move2.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Jump2.csv"));

	animation.SetTarget(&sprite);
	floor.setFillColor(sf::Color::Red);
	floor.setSize(sf::Vector2f(1920, 500));
	floor.setPosition(sf::Vector2f(0, 120.f));
	Utils::SetOrigin(floor, Origins::TC);
	SetOrigin(Origins::TC);
}

void Player2::Reset()
{
	animation.Play("Idle2");
	SetOrigin(origin);
	SetPosition({ 0, 0 });
	SetFlipX(false);
}

void Player2::Update(float dt)
{
	animation.Update(dt);
	float h = INPUT_MGR.GetAxis(Axis::Horizontal);

	// 플립
	if (h != 0.f)
	{
		bool flip = h < 0.f;
		if (GetFlipX() != flip)
		{
			SetFlipX(flip);
		}
	}

	// 점프
	if (isGround && INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		velocity.y += JumpForce;
		animation.Play("Jump2");
		isGround = false;
	}

	// 이동
	velocity.x = h * speed;
	velocity.y += gravity * dt;
	position += velocity * dt;

	// 바닥 충돌 처리
	if (position.y > 0.f)
	{
		isGround = true;
		position.y = 0.f;
		velocity.y = 0.f;
	}

	SetPosition(position);

	// 에니메이션
	if (animation.GetCurrentClipId() == "Idle2")
	{
		if (isGround && h != 0.f)
		{
			animation.Play("Move2");
		}
	}
	else if (animation.GetCurrentClipId() == "Move2")
	{
		if (isGround && h == 0.f)
		{
			animation.Play("Idle2");
		}
	}
	else if (animation.GetCurrentClipId() == "Jump2")
	{
		if (isGround)
		{
			animation.Play((h == 0.f) ? "Idle2" : "Move2");
		}
	}
}

bool Player2::GetFlipX() const
{
	return filpX;
}

void Player2::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !filpX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}

void Player2::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	//window.draw(floor);
}