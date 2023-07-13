#include "stdafx.h"
#include "AnimationController.h"
#include "ResourceMgr.h"



void AnimationController::AddClip(const AnimationClip& newClip)
{
	if (clips.find(newClip.id) == clips.end())
	{
		clips.insert({ newClip.id, newClip });
	}
}


void AnimationController::Update(float dt)
{
	if (!isPlaying)
		return;

	accumTime += dt * speed;
	if (accumTime < clipDuration)
		return;

	accumTime = 0.f;
	++currentFrame;

	if (currentFrame == totalFrame)
	{
		if (!queue.empty())
		{
			std::string id = queue.front();
			queue.pop();
			Play(id, false);
			return;
		}
		else
		{
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = totalFrame - 1;
				return;
			case AnimationLoopTypes::Loop:
				currentFrame = 0;
				break;
			}
		}
	}
	
	if (currentClip->frames[currentFrame].action != nullptr)
	{
		currentClip->frames[currentFrame].action();
	}
	
	if (target != nullptr)
	{
		SetFrame(currentClip->frames[currentFrame]);
	}
}

void AnimationController::Play(const std::string& clipid, bool clearQueue)
{
	if (clearQueue)
	{
		while(!queue.empty())
			queue.pop(); //팝 호출하면 제일 먼저 나왔던 놈이 빠짐
	}


	//재생할 플레이 정보를 가져와서 리셋해줌
	auto find = clips.find(clipid);
	if (find == clips.end())
	{
		Stop();
		std::cout << "ERR : NOT EXIST CLIP" << std::endl;
		return;
	}
	//std::string jump = "Jump";
	//if(clipid == jump)
	//std::cout << "안녕" << std::endl;


	isPlaying = true;
	currentClip = &find->second;
	currentFrame = 0;
	totalFrame = currentClip->frames.size();
	clipDuration = 1.f / currentClip->fps;
	accumTime = 0.f;

	SetFrame(currentClip->frames[currentFrame]);

}

void AnimationController::PlayQueue(const std::string& clipid)
{
	queue.push(clipid);
}
//코딩은 ㄴ재밌다 <- 누구야
void AnimationController::Stop()
{
	isPlaying = false;
}

void AnimationController::SetFrame(const AnimationFrame& frame)
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(frame.textureId);
	target->setTexture(*tex);
	target->setTextureRect(frame.texCoord);
}

std::string AnimationController::GetCurrentClipId()
{
	return currentClip->id;
}
