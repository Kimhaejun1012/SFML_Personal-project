#pragma once
#include "DataTable.h"
#include "MapStructure.h"

struct MapStructureInfo
{
	MapStructure::Types structureType;

	std::string textureId;
};


class MapStructureTable : public DataTable
{

protected:
	std::unordered_map<MapStructure::Types, MapStructureInfo> table;

public:
	MapStructureTable() : DataTable(DataTable::Ids::MapStructure) {};
	virtual ~MapStructureTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const MapStructureInfo& Get(MapStructure::Types id);
};

