#include "stdafx.h"
#include "Element.h"

Element::Element(const std::string& textureId, const std::string& n) : SpriteGo(textureId, n)
{
}

Element::~Element()
{
	Release();
}

void Element::Init()
{
	SpriteGo::Init();
}

void Element::Release()
{
	SpriteGo::Release();
}

void Element::Reset()
{
	SpriteGo::Reset();
}

void Element::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Element::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
