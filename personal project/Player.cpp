#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"
#include "SceneDev1.h"

void Player::Init()
{
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "tables/Idle.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "tables/Move.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "tables/Up.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "tables/Down.csv");



	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Idle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Move.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Up.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Down.csv"));

	animation.SetTarget(&sprite);

	SetOrigin(Origins::BC);

}

void Player::Reset()
{
	animation.Play("Idle");
	SetOrigin(origin);
	SetPosition({ 0, 500 });
	SetFlipX(false);
	sprite.setScale(0.5f, 0.5f);
}

void Player::Update(float dt)
{

	animation.Update(dt);
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y= INPUT_MGR.GetAxis(Axis::Vertical);
	// 플립
	if (direction.x != 0.f)
	{
		bool flip = direction.x > 0.f;
		if (GetFlipX() != flip)
		{
			SetFlipX(flip);
		}
	}

	// 이동

		velocity.x = direction.x * speed;
		velocity.y = direction.y * speed;
		position += velocity * dt;


		SetPosition(position);

		//sprite.getGlobalBounds().intersects(wallBounds,playerwall);
		//if (!wallBounds.contains(position))
		if(sprite.getGlobalBounds().intersects(wallBounds))
		{
			//std::cout << "충돌함" << std::endl;
			position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
		}

		// 에니메이션
		if (animation.GetCurrentClipId() == "Idle" || animation.GetCurrentClipId() == "Up" || animation.GetCurrentClipId() == "Down")
		{
			if (direction.x != 0.f)
			{
				animation.Play("Move");
			}
		}
		else if (animation.GetCurrentClipId() == "Move")
		{
			if (direction.x == 0.f)
			{
				animation.Play("Idle");
			}
		}

		if ((animation.GetCurrentClipId() == "Idle" || animation.GetCurrentClipId() == "Down" || animation.GetCurrentClipId() == "Move") && direction.x == 0)
		{
			//animation.GetCurrentClipId() = "Up";
			if (direction.y < 0.f)
			{
				animation.Play("Up");
			}
		}
		else if (animation.GetCurrentClipId() == "Up")
		{
			if (direction.y == 0.f)
			{
				animation.Play("Idle");
			}
		}
		if ((animation.GetCurrentClipId() == "Idle" || animation.GetCurrentClipId() == "Up" || animation.GetCurrentClipId() == "Move") && direction.x == 0)
		{
			if (direction.y > 0.f)
			{
				animation.Play("Down");
			}
		}
		else if (animation.GetCurrentClipId() == "Down")
		{
			if (direction.y == 0.f)
			{
				animation.Play("Idle");
			}
		}
}

bool Player::GetFlipX() const
{
	return filpX;
}


void Player::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !filpX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}


void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;
	wallBoundsLT = { wallBounds.left + 56 , wallBounds.top + 270 };
	wallBoundsRB = { wallBounds.left + wallBounds.width - 56, wallBounds.top + wallBounds.height - 270};
}