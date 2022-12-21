#pragma once
enum class STAGE_TYPE
{
	INTRO,
	HOUSE,
	FARM,
	GARAGE,
	STREET,
	COW,
	CHICKEN,
	MOUNTAIN,
	SHOP,

	END
};

constexpr int STAGE_TYPE_COUNT = static_cast<int>(STAGE_TYPE::END);

enum class OBJECT_TYPE
{
	TILE,
	ITEM,
	OBSTACLE,
	NPC,
	ANIMAL,
	PLOT,
	PLAYER,
	RAYCAST,
	TELEPORTER,
	INVOKER,

	UI = 31,

	END // = 32
};

constexpr int OBJECT_TYPE_COUNT = static_cast<int>(OBJECT_TYPE::END);

enum class COMPONENT_TYPE
{
	TRANSFORM,
	SPRITE,
	ANIMATOR,
	COLLIDER,
	MOVE,
	FSM,

	END
};

constexpr int COMPONENT_TYPE_COUNT = static_cast<int>(COMPONENT_TYPE::END);
