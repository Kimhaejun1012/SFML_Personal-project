#pragma once
#include "Singleton.h"

class ResourceMgr : public Singleton<ResourceMgr>
{
	friend Singleton<ResourceMgr>;

protected:
	ResourceMgr() = default;
	virtual ~ResourceMgr() override;
	
	std::string defaultFontId = "fonts/Noto_Sans_KR/NotoSansKR-Bold.otf";
	sf::Font* defaultFont = nullptr;

	std::map<std::string, std::tuple<sf::Texture*, bool>> mapTexture;
	std::map<std::string, std::tuple<sf::Font*, bool>> mapFont;
	std::map<std::string, std::tuple<sf::SoundBuffer*, bool>> mapSoundBuffer;
	std::map<std::string, std::tuple<AnimationClip*, bool>> mapAnimationClip;

public:

	void Init();

	void UnLoadAll();
	void LoadFromCSV(const std::string path, bool isDefault = false);
	void Load(ResourceTypes t, const std::string path, bool isDefault = false);
	void Unload(ResourceTypes t, const std::string path);

	sf::Texture* GetTexture(const std::string& id);
	sf::Font* GetFont(const std::string& id);
	sf::SoundBuffer* GetSoundBuffer(const std::string& id);
	AnimationClip* GetAnimationClip(const std::string& id);

	sf::Font* GetDeaultFont() { return defaultFont; }
};

#define RESOURCE_MGR (ResourceMgr::Instance())
