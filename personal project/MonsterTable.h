#pragma once
#include "DataTable.h"
#include "Monster.h"

struct MonsterInfo
{
	Monster::Types zombieType;

	std::string textureId;
	float speed;
	int maxHP;
	int damage;
	float attackRate;
};

class MonsterTable : public DataTable
{
protected:
	std::unordered_map<Monster::Types, MonsterInfo> table;

public:
	MonsterTable() : DataTable(DataTable::Ids::Monster) {};
	virtual ~MonsterTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const MonsterInfo& Get(Monster::Types id);

};

