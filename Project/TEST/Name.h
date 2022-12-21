#pragma once
#include <string>

#include "CItem.h"

namespace PLAYER
{
	namespace GROUPS
	{
		enum GROUPS_ENUM
		{
			IDLE_0,
			IDLE_1,
			IDLE_2,
			WALK_0,
			RUN_0,
			JUMP_0,
			SELECT_0,
			CARRY_0,
			CARRY_IDLE_0,
			CARRYING_WALK_0,

			CARRYING_RUN_0,
			CARRYING_JUMP_0,
			CARRYING_THROW_0,
			FEED_0,
			BELL_0,
			NORMAL_SICKLE_0,
			NORMAL_HAMMER_0,
			SPECIAL_HAMMER_0,
			NORMAL_WATER_0,
			NORMAL_AXE_0,

			SPECIAL_AXE_0,
			NORMAL_HOE_0,
			SPECIAL_HOE_0,
			BRUSH_0,
			MILKING_0,
			WHISTLE_0,

			END
		};

		constexpr int GROUP_COUNT = END;

		std::array<std::wstring, GROUP_COUNT> ANIMATIONS =
		{
			L"IDLE_0",
			L"IDLE_1",
			L"IDLE_2",
			L"WALK_0",
			L"RUN_0",
			L"JUMP_0",
			L"SELECT_0",
			L"CARRY_0",
			L"CARRY_IDLE_0",
			L"CARRYING_WALK_0",

			L"CARRYING_RUN_0",
			L"CARRYING_JUMP_0",
			L"CARRYING_THROW_0",
			L"FEED_0",
			L"BELL_0",
			L"NORMAL_SICKLE_0",
			L"NORMAL_HAMMER_0",
			L"SPECIAL_HAMMER_0",
			L"NORMAL_WATER_0",
			L"NORMAL_AXE_0",

			L"SPECIAL_AXE_0",
			L"NORMAL_HOE_0",
			L"SPECIAL_HOE_0",
			L"BRUSH_0",
			L"MILKING_0",
			L"WHISTLE_0",
		};
	}

	enum PLAYER_ENUM
	{
		//IDLE_0
		IDLE_LEFT_0,
		IDLE_RIGHT_0,
		IDLE_BACK_0,
		IDLE_FRONT_0,

		//IDLE_1
		IDLE_LEFT_1,
		IDLE_RIGHT_1,
		IDLE_BACK_1,
		IDLE_FRONT_1,

		//IDLE_2
		IDLE_LEFT_2,
		IDLE_RIGHT_2,
		IDLE_BACK_2,
		IDLE_FRONT_2,

		//WALK
		WALK_LEFT_0,
		WALK_RIGHT_0,
		WALK_BACK_0,
		WALK_FRONT_0,

		//RUN
		RUN_LEFT_0,
		RUN_RIGHT_0,
		RUN_BACK_0,
		RUN_FRONT_0,

		//JUMP
		JUMP_LEFT_0,
		JUMP_RIGHT_0,
		JUMP_BACK_0,
		JUMP_FRONT_0,

		//SELECT
		SELECT_LEFT_0,
		SELECT_RIGHT_0,
		SELECT_BACK_0,
		SELECT_FRONT_0,

		//EAT
		EAT_FRONT_0,

		//EXHAUSTED
		EXHAUSTED_LEVEL_0,
		EXHAUSTED_LEVEL_1,
		EXHAUSTED_LEVEL_2,
		EXHAUSTED_LEVEL_3,

		//CARRY
		CARRY_LEFT_0,
		CARRY_RIGHT_0,
		CARRY_BACK_0,
		CARRY_FRONT_0,

		//CARRY_IDLE
		CARRY_IDLE_LEFT_0,
		CARRY_IDLE_RIGHT_0,
		CARRY_IDLE_BACK_0,
		CARRY_IDLE_FRONT_0,

		//CARRYING_WALK
		CARRYING_WALK_LEFT_0,
		CARRYING_WALK_RIGHT_0,
		CARRYING_WALK_BACK_0,
		CARRYING_WALK_FRONT_0,

		//CARRYING_RUN
		CARRYING_RUN_LEFT_0,
		CARRYING_RUN_RIGHT_0,
		CARRYING_RUN_BACK_0,
		CARRYING_RUN_FRONT_0,

		//CARRYING_JUMP
		CARRYING_JUMP_LEFT_0,
		CARRYING_JUMP_RIGHT_0,
		CARRYING_JUMP_BACK_0,
		CARRYING_JUMP_FRONT_0,

		//CARRYING_THROW
		CARRYING_THROW_LEFT_0,
		CARRYING_THROW_RIGHT_0,
		CARRYING_THROW_BACK_0,
		CARRYING_THROW_FRONT_0,

		//FEED
		FEED_LEFT_0,
		FEED_RIGHT_0,
		FEED_BACK_0,
		FEED_FRONT_0,

		//PAINT
		PAINT_BACK_0,
		//SEED
		SEED_FRONT_0,

		//BELL
		BELL_LEFT_0,
		BELL_RIGHT_0,
		BELL_FRONT_0,
		BELL_BACK_0,

		//NORMAL SICKLE
		NORMAL_SICKLE_LEFT_0,
		NORMAL_SICKLE_RIGHT_0,
		NORMAL_SICKLE_BACK_0,
		NORMAL_SICKLE_FRONT_0,

		//SPECIAL_SICKLE
		SPECIAL_SICKLE_FRONT_0,

		//NORMAL_HAMMER
		NORMAL_HAMMER_LEFT_0,
		NORMAL_HAMMER_RIGHT_0,
		NORMAL_HAMMER_BACK_0,
		NORMAL_HAMMER_FRONT_0,

		//SPECIAL_HAMMER
		SPECIAL_HAMMER_LEFT_0,
		SPECIAL_HAMMER_RIGHT_0,
		SPECIAL_HAMMER_BACK_0,
		SPECIAL_HAMMER_FRONT_0,

		//NORMAL_WATER
		NORMAL_WATER_LEFT_0,
		NORMAL_WATER_RIGHT_0,
		NORMAL_WATER_BACK_0,
		NORMAL_WATER_FRONT_0,

		//SPECIAL_WATER
		SPECIAL_WATER_FRONT_0,

		//NORMAL_AXE
		NORMAL_AXE_LEFT_0,
		NORMAL_AXE_RIGHT_0,
		NORMAL_AXE_BACK_0,
		NORMAL_AXE_FRONT_0,

		//SPECIAL_AXE
		SPECIAL_AXE_LEFT_0,
		SPECIAL_AXE_RIGHT_0,
		SPECIAL_AXE_BACK_0,
		SPECIAL_AXE_FRONT_0,

		//NORMAL_HOE
		NORMAL_HOE_LEFT_0,
		NORMAL_HOE_RIGHT_0,
		NORMAL_HOE_BACK_0,
		NORMAL_HOE_FRONT_0,

		//SPECIAL_HOE
		SPECIAL_HOE_LEFT_0,
		SPECIAL_HOE_RIGHT_0,
		SPECIAL_HOE_BACK_0,
		SPECIAL_HOE_FRONT_0,

		//BRUSH
		BRUSH_LEFT_0,
		BRUSH_RIGHT_0,
		BRUSH_BACK_0,
		BRUSH_FRONT_0,

		//MILKING
		MILKING_LEFT_0,
		MILKING_RIGHT_0,
		MILKING_BACK_0,
		MILKING_FRONT_0,

		//WHISTLE
		WHISTLE_LEFT_0,
		WHISTLE_RIGHT_0,
		WHISTLE_FRONT_0,

		END
	};

	constexpr int PLAYER_COUNT = END;

	std::array<std::wstring, PLAYER_COUNT> ANIMATIONS =
	{
		//idle_0
		L"idle_left_0",
		L"idle_right_0",
		L"idle_back_0",
		L"idle_front_0",

		//idle_1
		L"idle_left_1",
		L"idle_right_1",
		L"idle_back_1",
		L"idle_front_1",

		//idle_2
		L"idle_left_2",
		L"idle_right_2",
		L"idle_back_2",
		L"idle_front_2",

		//walk
		L"walk_left_0",
		L"walk_right_0",
		L"walk_back_0",
		L"walk_front_0",

		//run
		L"run_left_0",
		L"run_right_0",
		L"run_back_0",
		L"run_front_0",

		//jump
		L"jump_left_0",
		L"jump_right_0",
		L"jump_back_0",
		L"jump_front_0",

		//select
		L"select_left_0",
		L"select_right_0",
		L"select_back_0",
		L"select_front_0",

		//eat
		L"eat_front_0",

		//exhausted
		L"exhausted_level_0",
		L"exhausted_level_1",
		L"exhausted_level_2",
		L"exhausted_level_3",

		//carrying
		L"carry_left_0",
		L"carry_right_0",
		L"carry_back_0",
		L"carry_front_0",

		//carrying_idle
		L"carry_idle_left_0",
		L"carry_idle_right_0",
		L"carry_idle_back_0",
		L"carry_idle_front_0",

		//carrying_walk
		L"carrying_walk_left_0",
		L"carrying_walk_right_0",
		L"carrying_walk_back_0",
		L"carrying_walk_front_0",

		//carrying_run
		L"carrying_run_left_0",
		L"carrying_run_right_0",
		L"carrying_run_back_0",
		L"carrying_run_front_0",

		//carrying_jump
		L"carrying_jump_left_0",
		L"carrying_jump_right_0",
		L"carrying_jump_back_0",
		L"carrying_jump_front_0",

		//carrying_throw
		L"throwing_left_0",
		L"throwing_right_0",
		L"throwing_back_0",
		L"throwing_front_0",
		//feed
		L"feed_left_0",
		L"feed_right_0",
		L"feed_back_0",
		L"feed_front_0",

		//paint
		L"paint_back_0",
		//seed
		L"seed_front_0",

		//bell
		L"bell_left_0",
		L"bell_right_0",
		L"bell_front_0",
		L"bell_back_0",

		//normal sickle
		L"normal_sickle_left_0",
		L"normal_sickle_right_0",
		L"normal_sickle_back_0",
		L"normal_sickle_front_0",

		//special_sickle
		L"special_sickle_front_0",

		//normal_hammer
		L"normal_hammer_left_0",
		L"normal_hammer_right_0",
		L"normal_hammer_back_0",
		L"normal_hammer_front_0",

		//special_hammer
		L"special_hammer_left_0",
		L"special_hammer_right_0",
		L"special_hammer_back_0",
		L"special_hammer_front_0",

		//normal_water
		L"normal_water_left_0",
		L"normal_water_right_0",
		L"normal_water_back_0",
		L"normal_water_front_0",

		//special_water
		L"special_water_front_0",

		//normal_axe
		L"normal_axe_left_0",
		L"normal_axe_right_0",
		L"normal_axe_back_0",
		L"normal_axe_front_0",

		//special_axe
		L"special_axe_left_0",
		L"special_axe_right_0",
		L"special_axe_back_0",
		L"special_axe_front_0",

		//normal_hoe
		L"normal_hoe_left_0",
		L"normal_hoe_right_0",
		L"normal_hoe_back_0",
		L"normal_hoe_front_0",

		//special_hoe
		L"special_hoe_left_0",
		L"special_hoe_right_0",
		L"special_hoe_back_0",
		L"special_hoe_front_0",

		//brush
		L"brush_left_0",
		L"brush_right_0",
		L"brush_back_0",
		L"brush_front_0",

		//milking
		L"milking_left_0",
		L"milking_right_0",
		L"milking_back_0",
		L"milking_front_0",

		//whistle
		L"whistle_left_0",
		L"whistle_right_0",
		L"whistle_front_0",
	};
}

namespace TOOL
{
	enum TOOL_ENUM
	{
		NORMAL_WATER,
		NORMAL_SICKLE,
		NORMAL_HOE,
		NORMAL_HAMMER,
		NORMAL_AXE,
		SPECIAL_WATER,
		SPECIAL_SICKLE,
		SPECIAL_HOE,
		SPECIAL_AXE,
		SPECIAL_HAMMER,
		MEDICINE,
		SPECIAL_MEDICINE,
		BELL,
		MILKING_TOOL,
		BRUSH,
		COW_FOOD,
		CHICKEN_FOOD,
		SEED_TURNIP,
		SEED_POTATO,
		SEED_TOMATO,
		SEED_CORN,
		SEED_GRASS,
		PAINT,
		SEED,

		END
	};

	constexpr int TOOL_COUNT = END;

	std::array<std::wstring, TOOL_COUNT> TYPES =
	{
		L"normal_water",
		L"normal_sickle",
		L"normal_hoe",
		L"normal_hammer",
		L"normal_axe",
		L"special_water",
		L"special_sickle",
		L"special_hoe",
		L"special_axe",
		L"special_hammer",
		L"medicine",
		L"special_medicine",
		L"bell",
		L"milking_tool",
		L"brush",
		L"cow_food",
		L"chicken_food",
		L"seed_turnip",
		L"seed_potato",
		L"seed_tomato",
		L"seed_corn",
		L"seed_grass",
		L"paint",
		L"seed",
	};
}

namespace PLOT
{
	enum PLOT_ENUM
	{
		EMPTY_PLOT,
		EMPTY_WATERED_PLOT,
		SEEDED_PLOT,
		SEEDED_WATERED_PLOT,
		SECRETED_PLOT,
		SECRETED_FLOWERED,
		TURNIP_0,
		TURNIP_1,
		TURNIP_2,
		TURNIP_WATERED_0,
		TURNIP_WATERED_1,
		TURNIP_3,
		POTATO_0,
		POTATO_1,
		POTATO_2,
		POTATO_3,
		POTATO_WATERED_0,
		POTATO_WATERED_1,
		TOMATO_0,
		TOMATO_1,
		TOMATO_2,
		TOMATO_3,
		TOMATO_4,
		TOMATO_5,
		TOMATO_WATERED_0,
		TOMATO_WATERED_1,
		TOMATO_WATERED_2,
		TOMATO_WATERED_3,
		CORN_0,
		CORN_1,
		CORN_2,
		CORN_3,
		CORN_4,
		CORN_5,
		CORN_WATERED_0,
		CORN_WATERED_1,
		CORN_WATERED_2,
		CORN_WATERED_3,

		END
	};

	constexpr int PLOT_COUNT = END;

	std::array<std::wstring, PLOT_COUNT> TYPES =
	{
		L"empty_plot",
		L"empty_watered_plot",
		L"seeded_plot",
		L"seeded_watered_plot",
		L"secreted_plot",
		L"secreted_flowered",
		L"turnip_0",
		L"turnip_1",
		L"turnip_2",
		L"turnip_watered_0",
		L"turnip_watered_1",
		L"turnip_3",
		L"potato_0",
		L"potato_1",
		L"potato_2",
		L"potato_3",
		L"potato_watered_0",
		L"potato_watered_1",
		L"tomato_0",
		L"tomato_1",
		L"tomato_2",
		L"tomato_3",
		L"tomato_4",
		L"tomato_5",
		L"tomato_watered_0",
		L"tomato_watered_1",
		L"tomato_watered_2",
		L"tomato_watered_3",
		L"corn_0",
		L"corn_1",
		L"corn_2",
		L"corn_3",
		L"corn_4",
		L"corn_5",
		L"corn_watered_0",
		L"corn_watered_1",
		L"corn_watered_2",
		L"corn_watered_3",
	};
}

namespace EXTRA
{
	enum EXTRA_ENUM
	{
		BIG_MILK,
		SMALL_MILK,
		FEED,
		SEED_TURNIP,
		SEED_TOMATO,
		SEED_GRASS,
		SEED_POTATO,
		SEED_CORN,
		FRUIT,
		FOOD_0,
		FOOD_1,
		FOOD_2,
		EGG,
		EGG_CRASHED,
		WOOD_CARRYING,
		WOOD,
		WOOD_CRASHED,
		GRASS_CRASHED,
		GRASS_0,
		GRASS_1,
		GRASS_2,
		GRASS_3,
		OBSTACLE_GRASS,
		OBSTACLE_SMALL_ROCK,
		OBSTACLE_BIG_ROCK,
		OBSTACLE_WOOD,

		END
	};

	constexpr int EXTRA_COUNT = END;

	std::array<std::wstring, EXTRA_COUNT> TYPES =
	{
		L"big_milk",
		L"small_milk",
		L"feed",
		L"seed_turnip",
		L"seed_tomato",
		L"seed_grass",
		L"seed_potato",
		L"seed_corn",
		L"fruit",
		L"food_0",
		L"food_1",
		L"food_2",
		L"egg",
		L"egg_crashed",
		L"wood_carrying",
		L"wood",
		L"wood_crashed",
		L"grass_crashed",
		L"grass_0",
		L"grass_1",
		L"grass_2",
		L"grass_3",
		L"obstacle_grass",
		L"obstacle_small_rock",
		L"obstacle_big_rock",
		L"obstacle_wood",
	};
}

namespace BASIC_INFO
{
	std::map<ItemID, ItemInfo> MAP_DATA =
	{
		{
			{TOOL::NORMAL_AXE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::NORMAL_AXE, ITEM_CATEGORY::TOOL},
				{0},
				{-2},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::SPECIAL_AXE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SPECIAL_AXE, ITEM_CATEGORY::TOOL},
				{2000},
				{-8},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::NORMAL_HAMMER, ITEM_CATEGORY::TOOL},
			{
				{TOOL::NORMAL_HAMMER, ITEM_CATEGORY::TOOL},
				{0},
				{-2},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::SPECIAL_HAMMER, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SPECIAL_HAMMER, ITEM_CATEGORY::TOOL},
				{2000},
				{-4},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::NORMAL_HOE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::NORMAL_HOE, ITEM_CATEGORY::TOOL},
				{0},
				{-2},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::SPECIAL_HOE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SPECIAL_HOE, ITEM_CATEGORY::TOOL},
				{2000},
				{-8},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::NORMAL_SICKLE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::NORMAL_SICKLE, ITEM_CATEGORY::TOOL},
				{0},
				{-2},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::SPECIAL_SICKLE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SPECIAL_SICKLE, ITEM_CATEGORY::TOOL},
				{2000},
				{-8},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::NORMAL_WATER, ITEM_CATEGORY::TOOL},
			{
				{TOOL::NORMAL_WATER, ITEM_CATEGORY::TOOL},
				{0},
				{-2},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::SPECIAL_WATER, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SPECIAL_WATER, ITEM_CATEGORY::TOOL},
				{2000},
				{-8},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::BRUSH, ITEM_CATEGORY::TOOL},
			{
				{TOOL::BRUSH, ITEM_CATEGORY::TOOL},
				{400},
				{-1},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::MILKING_TOOL, ITEM_CATEGORY::TOOL},
			{
				{TOOL::MILKING_TOOL, ITEM_CATEGORY::TOOL},
				{1400},
				{0},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::PAINT, ITEM_CATEGORY::TOOL},
			{
				{TOOL::PAINT, ITEM_CATEGORY::TOOL},
				{1500},
				{-2},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::SEED_GRASS, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SEED_GRASS, ITEM_CATEGORY::TOOL},
				{500},
				{-1},
				{999},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
		{
			{TOOL::SEED_POTATO, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SEED_POTATO, ITEM_CATEGORY::TOOL},
				{200},
				{-1},
				{999},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
		{
			{TOOL::SEED_TURNIP, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SEED_TURNIP, ITEM_CATEGORY::TOOL},
				{200},
				{-1},
				{999},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
		{
			{TOOL::SEED_TOMATO, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SEED_TOMATO, ITEM_CATEGORY::TOOL},
				{300},
				{-1},
				{999},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
		{
			{TOOL::SEED_CORN, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SEED_CORN, ITEM_CATEGORY::TOOL},
				{300},
				{-1},
				{999},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
		{
			{TOOL::BELL, ITEM_CATEGORY::TOOL},
			{
				{TOOL::BELL, ITEM_CATEGORY::TOOL},
				{0},
				{-1},
				{1},
				{1},
				{false},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::MEDICINE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::MEDICINE, ITEM_CATEGORY::TOOL},
				{1000},
				{-1},
				{1},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
		{
			{EXTRA::SMALL_MILK, ITEM_CATEGORY::SELL},
			{
				{EXTRA::SMALL_MILK, ITEM_CATEGORY::SELL},
				{150},
				{0},
				{999},
				{1},
				{true},
				{false},
				{false},
				{}
			},
		},
		{
			{EXTRA::BIG_MILK, ITEM_CATEGORY::SELL},
			{
				{EXTRA::BIG_MILK, ITEM_CATEGORY::SELL},
				{350},
				{0},
				{999},
				{1},
				{true},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::CHICKEN_FOOD, ITEM_CATEGORY::TOOL},

			{
				{TOOL::CHICKEN_FOOD, ITEM_CATEGORY::TOOL},
				{300},
				{-2},
				{12},
				{12},
				{false},
				{false},
				{true},
				{180, 500}
			},
		},
		{
			{TOOL::COW_FOOD, ITEM_CATEGORY::TOOL},
			{
				{TOOL::COW_FOOD, ITEM_CATEGORY::TOOL},
				{1000},
				{-2},
				{12},
				{12},
				{false},
				{false},
				{true},
				{180, 860}
			},
		},
		{
			{TOOL::SPECIAL_MEDICINE, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SPECIAL_MEDICINE, ITEM_CATEGORY::TOOL},
				{4000},
				{-1},
				{1},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
		{
			{PLOT::TOMATO_4, ITEM_CATEGORY::SELL},
			{
				{PLOT::TOMATO_4, ITEM_CATEGORY::SELL},
				{100},
				{0},
				{999},
				{1},
				{true},
				{false},
				{false},
				{}
			},
		},
		{
			{PLOT::POTATO_2, ITEM_CATEGORY::SELL},
			{
				{PLOT::POTATO_2, ITEM_CATEGORY::SELL},
				{80},
				{0},
				{999},
				{1},
				{true},
				{false},
				{false},
				{}
			},
		},
		{
			{PLOT::TURNIP_2, ITEM_CATEGORY::SELL},
			{
				{PLOT::TURNIP_2, ITEM_CATEGORY::SELL},
				{60},
				{0},
				{1},
				{1},
				{true},
				{false},
				{false},
				{}
			},
		},
		{
			{EXTRA::EGG, ITEM_CATEGORY::SELL},

			{
				{EXTRA::EGG, ITEM_CATEGORY::SELL},
				{50},
				{0},
				{1},
				{1},
				{true},
				{false},
				{false},
				{}
			},
		},
		{
			{EXTRA::FRUIT, ITEM_CATEGORY::DEFAULT},
			{
				{EXTRA::FRUIT, ITEM_CATEGORY::DEFAULT},
				{150},
				{10},
				{999},
				{1},
				{true},
				{true},
				{false},
				{}
			},

		},
		{
			{EXTRA::WOOD_CARRYING, ITEM_CATEGORY::DEFAULT},
			{
				{EXTRA::WOOD_CARRYING, ITEM_CATEGORY::DEFAULT},
				{0},
				{0},
				{999},
				{1},
				{false},
				{false},
				{true},
				{}
			},

		},

		{
			{EXTRA::FOOD_0, ITEM_CATEGORY::DEFAULT},
			{
				{EXTRA::FOOD_0, ITEM_CATEGORY::DEFAULT},
				{0},
				{0},
				{1},
				{1},
				{false},
				{true},
				{true},
				{}
			},

		},
		{
			{PLOT::CORN_4, ITEM_CATEGORY::SELL},
			{
				{PLOT::CORN_4, ITEM_CATEGORY::SELL},
				{120},
				{0},
				{999},
				{1},
				{true},
				{false},
				{false},
				{}
			},
		},
		{
			{TOOL::SEED, ITEM_CATEGORY::TOOL},
			{
				{TOOL::SEED, ITEM_CATEGORY::TOOL},
				{50},
				{0},
				{999},
				{1},
				{false},
				{false},
				{true},
				{}
			},
		},
	};
}

namespace CHICKEN
{
	namespace GROUPS
	{
		enum GROUP_ENUM
		{
			WALK_0,
			CHICK_WALK_0,

			END
		};

		constexpr int GROUP_COUNT = END;

		std::array<std::wstring, GROUP_COUNT> ANIMATIONS =
		{
			L"walk_0",
			L"chick_walk_0",
		};
	}

	enum CHICKEN_ENUM
	{
		WALK_FRONT_0,
		WALK_LEFT_0,
		WALK_RIGHT_0,
		WALK_BACK_0,

		IDLE_LEFT_0,
		IDLE_RIGHT_0,

		CHICK_WALK_FRONT_0,
		CHICK_WALK_LEFT_0,
		CHICK_WALK_RIGHT_0,
		CHICK_WALK_BACK_0,

		END
	};

	constexpr int CHICKEN_COUNT = END;

	std::array<std::wstring, CHICKEN_COUNT> ANIMATIONS =
	{
		L"walk_front_0",
		L"walk_left_0",
		L"walk_right_0",
		L"walk_back_0",

		L"idle_left_0",
		L"idle_right_0",

		L"chick_walk_front_0",
		L"chick_walk_left_0",
		L"chick_walk_right_0",
		L"chick_walk_back_0",
	};
}

namespace COW
{
	namespace GROUPS
	{
		enum
		{
			IDLE_0,
			IDLE_1,
			IDLE_2,
			IDLE_3,
			IDLE_4,

			IDLE_FRUSTRATED_0,
			IDLE_FRUSTRATED_1,
			IDLE_FRUSTRATED_2,
			IDLE_FRUSTRATED_3,
			IDLE_FRUSTRATED_4,

			IDLE_SICK_0,
			IDLE_SICK_1,
			IDLE_SICK_2,
			IDLE_SICK_3,

			IDLE_HAPPY_0,

			WALK_0,

			WALK_SICK_0,

			WALK_FRUSTRATED_0,

			IDLE_TEEN_0,
			IDLE_TEEN_1,
			IDLE_TEEN_2,
			IDLE_TEEN_3,

			IDLE_TEEN_SICK_0,

			IDLE_TEEN_HAPPY_0,

			IDLE_TEEN_FRUSTRATED_0,

			WALK_TEEN_0,

			WALK_TEEN_FRUSTRATED_0,

			IDLE_CHILD_0,
			IDLE_CHILD_1,
			IDLE_CHILD_2,

			IDLE_CHILD_FRUSTRATED_0,

			WALK_CHILD_0,
			WALK_CHILD_1,

			WALK_CHILD_FRUSTRATED_0,

			IDLE_PREGNANT_0,
			IDLE_PREGNANT_1,
			IDLE_PREGNANT_2,

			WALK_PREGNANT_0,

			END
		};

		constexpr int GROUP_COUNT = END;

		std::array<std::wstring, GROUP_COUNT> ANIMATIONS =
		{
			L"IDLE_0",
			L"IDLE_1",
			L"IDLE_2",
			L"IDLE_3",
			L"IDLE_4",

			L"IDLE_FRUSTRATED_0",
			L"IDLE_FRUSTRATED_1",
			L"IDLE_FRUSTRATED_2",
			L"IDLE_FRUSTRATED_3",
			L"IDLE_FRUSTRATED_4",

			L"IDLE_SICK_0",
			L"IDLE_SICK_1",
			L"IDLE_SICK_2",
			L"IDLE_SICK_3",

			L"IDLE_HAPPY_0",

			L"WALK_0",
			L"WALK_SICK_0",
			L"WALK_FRUSTRATED_0",

			L"IDLE_TEEN_0",
			L"IDLE_TEEN_1",
			L"IDLE_TEEN_2",
			L"IDLE_TEEN_3",
			L"IDLE_TEEN_SICK_0",
			L"IDLE_TEEN_HAPPY_0",
			L"IDLE_TEEN_FRUSTRATED_0",

			L"WALK_TEEN_0",
			L"WALK_TEEN_FRUSTRATED_0",

			L"IDLE_CHILD_0",
			L"IDLE_CHILD_1",
			L"IDLE_CHILD_2",
			L"IDLE_CHILD_FRUSTRATED_0",

			L"WALK_CHILD_0",
			L"WALK_CHILD_1",
			L"WALK_CHILD_FRUSTRATED_0",

			L"IDLE_PREGNANT_0",
			L"IDLE_PREGNANT_1",
			L"IDLE_PREGNANT_2",

			L"WALK_PREGNANT_0",
		};
	}

	enum COW_ENUM
	{
		//CHILD
		IDLE_CHILD_LEFT_0,
		IDLE_CHILD_RIGHT_0,
		IDLE_CHILD_BACK_0,
		IDLE_CHILD_FRONT_0,

		IDLE_CHILD_LEFT_1,
		IDLE_CHILD_LEFT_2,

		IDLE_CHILD_RIGHT_1,
		IDLE_CHILD_RIGHT_2,

		IDLE_CHILD_FRUSTRATED_FRONT_0,
		IDLE_CHILD_FRUSTRATED_LEFT_0,
		IDLE_CHILD_FRUSTRATED_RIGHT_0,

		WALK_CHILD_LEFT_0,
		WALK_CHILD_RIGHT_0,
		WALK_CHILD_BACK_0,
		WALK_CHILD_FRONT_0,

		WALK_CHILD_FRUSTRATED_FRONT_0,
		WALK_CHILD_FRUSTRATED_LEFT_0,
		WALK_CHILD_FRUSTRATED_RIGHT_0,

		WALK_CHILD_SICK_LEFT_0,
		WALK_CHILD_SICK_RIGHT_0,

		RUN_CHILD_HAPPY_LEFT_0,
		RUN_CHILD_HAPPY_RIGHT_0,

		RUN_CHILD_LEFT_0,
		RUN_CHILD_RIGHT_0,

		//ADULT
		IDLE_LEFT_0,
		IDLE_RIGHT_0,
		IDLE_FRONT_0,
		IDLE_BACK_0,

		IDLE_LEFT_1,
		IDLE_RIGHT_1,
		IDLE_FRONT_1,

		IDLE_LEFT_2,
		IDLE_RIGHT_2,
		IDLE_FRONT_2,

		IDLE_LEFT_3,
		IDLE_RIGHT_3,

		IDLE_LEFT_4,
		IDLE_RIGHT_4,

		WALK_BACK_0,
		WALK_FRONT_0,
		WALK_RIGHT_0,
		WALK_LEFT_0,

		//FRUSTRATED
		IDLE_FRUSTRATED_LEFT_0,
		IDLE_FRUSTRATED_RIGHT_0,
		IDLE_FRUSTRATED_FRONT_0,

		IDLE_FRUSTRATED_LEFT_1,
		IDLE_FRUSTRATED_RIGHT_1,

		IDLE_FRUSTRATED_LEFT_2,
		IDLE_FRUSTRATED_RIGHT_2,

		IDLE_FRUSTRATED_LEFT_3,
		IDLE_FRUSTRATED_RIGHT_3,

		IDLE_FRUSTRATED_LEFT_4,
		IDLE_FRUSTRATED_RIGHT_4,

		WALK_FRUSTRATED_FRONT_0,
		WALK_FRUSTRATED_LEFT_0,
		WALK_FRUSTRATED_RIGHT_0,

		//SICK
		IDLE_SICK_LEFT_0,
		IDLE_SICK_RIGHT_0,
		IDLE_SICK_FRONT_0,

		IDLE_SICK_LEFT_1,
		IDLE_SICK_RIGHT_1,

		IDLE_SICK_LEFT_2,
		IDLE_SICK_RIGHT_2,

		IDLE_SICK_LEFT_3,
		IDLE_SICK_RIGHT_3,

		WALK_SICK_LEFT_0,
		WALK_SICK_RIGHT_0,

		//HAPPY
		IDLE_HAPPY_FRONT_0,
		IDLE_HAPPY_LEFT_0,
		IDLE_HAPPY_RIGHT_0,

		//PREGNANT
		IDLE_PREGNANT_LEFT_0,
		IDLE_PREGNANT_RIGHT_0,
		IDLE_PREGNANT_BACK_0,
		IDLE_PREGNANT_FRONT_0,

		IDLE_PREGNANT_LEFT_1,
		IDLE_PREGNANT_RIGHT_1,

		IDLE_PREGNANT_HAPPY_FRONT_0,

		WALK_PREGNANT_LEFT_0,
		WALK_PREGNANT_RIGHT_0,
		WALK_PREGNANT_BACK_0,
		WALK_PREGNANT_FRONT_0,

		//TEEN
		IDLE_TEEN_BACK_0,
		IDLE_TEEN_FRONT_0,

		IDLE_TEEN_FRUSTRATED_LEFT_0,
		IDLE_TEEN_FRUSTRATED_RIGHT_0,
		IDLE_TEEN_FRUSTRATED_FRONT_0,

		IDLE_TEEN_HAPPY_LEFT_0,
		IDLE_TEEN_HAPPY_RIGHT_0,

		IDLE_TEEN_LEFT_1,
		IDLE_TEEN_RIGHT_1,

		IDLE_TEEN_LEFT_2,
		IDLE_TEEN_RIGHT_2,

		IDLE_TEEN_LEFT_3,
		IDLE_TEEN_RIGHT_3,

		IDLE_TEEN_SICK_LEFT_0,
		IDLE_TEEN_SICK_RIGHT_0,

		RUN_TEEN_LEFT_0,
		RUN_TEEN_RIGHT_0,

		WALK_TEEN_BACK_0,
		WALK_TEEN_FRONT_0,

		WALK_TEEN_FRUSTRATED_LEFT_0,
		WALK_TEEN_FRUSTRATED_RIGHT_0,
		WALK_TEEN_FRUSTRATED_FRONT_0,

		END
	};

	constexpr int COW_COUNT = END;

	std::array<std::wstring, COW_COUNT> ANIMATIONS =
	{
		//CHILD
		L"idle_child_left_0",
		L"idle_child_right_0",
		L"idle_child_back_0",
		L"idle_child_front_0",

		L"idle_child_left_1",
		L"idle_child_left_2",

		L"idle_child_right_1",
		L"idle_child_right_2",

		L"idle_child_frustrated_front_0",
		L"idle_child_frustrated_left_0",
		L"idle_child_frustrated_right_0",

		L"walk_child_left_0",
		L"walk_child_right_0",
		L"walk_child_back_0",
		L"walk_child_front_0",

		L"walk_child_frustrated_front_0",
		L"walk_child_frustrated_left_0",
		L"walk_child_frustrated_right_0",

		L"walk_child_sick_left_0",
		L"walk_child_sick_right_0",

		L"run_child_happy_left_0",
		L"run_child_happy_right_0",

		L"run_child_left_0",
		L"run_child_right_0",

		//ADULT
		L"idle_left_0",
		L"idle_right_0",
		L"idle_front_0",
		L"idle_back_0",

		L"idel_left_1",
		L"idle_right_1",
		L"idle_front_1",

		L"idle_left_2",
		L"idle_right_2",
		L"idle_front_2",

		L"idle_left_3",
		L"idle_right_3",

		L"idle_left_4",
		L"idle_right_4",

		L"walk_back_0",
		L"walk_front_0",
		L"walk_right_0",
		L"walk_left_0",

		//FRUSTRATED
		L"idle_frustrated_left_0",
		L"idle_frustrated_right_0",
		L"idle_frustrated_front_0",

		L"idle_frustrated_left_1",
		L"idle_frustrated_right_1",

		L"idle_frustrated_left_2",
		L"idle_frustrated_right_2",

		L"idle_frustrated_left_3",
		L"idle_frustrated_right_3",

		L"idle_frustrated_left_4",
		L"idle_frustrated_right_4",

		L"walk_frustrated_front_0",
		L"walk_frustrated_left_0",
		L"walk_frustrated_right_0",

		//SICK
		L"idle_sick_left_0",
		L"idle_sick_right_0",
		L"idle_sick_front_0",

		L"idle_sick_left_1",
		L"idle_sick_right_1",

		L"idle_sick_left_2",
		L"idle_sick_right_2",

		L"idle_sick_left_3",
		L"idle_sick_right_3",

		L"walk_sick_left_0",
		L"walk_sick_right_0",

		//HAPPY
		L"idle_happy_front_0",
		L"idle_happy_left_0",
		L"idle_happy_right_0",

		//PREGNANT
		L"idle_pregnant_left_0",
		L"idle_pregnant_right_0",
		L"idle_pregnant_back_0",
		L"idle_pregant_front_0",

		L"idle_pregnant_left_1",
		L"idle_pregnant_right_1",

		L"idle_pregant_happy_front_0",

		L"walk_pregnant_left_0",
		L"walk_pregnant_right_0",
		L"walk_pregnant_back_0",
		L"walk_pregnant_front_0",

		//TEEN
		L"idle_teen_back_0",
		L"idle_teen_front_0",

		L"idle_teen_frustrated_left_0",
		L"idle_teen_frustrated_right_0",
		L"idle_teen_frustrated_front_0",

		L"idle_teen_happy_left_0",
		L"idle_teen_happy_right_0",

		L"idle_teen_left_1",
		L"idle_teen_right_1",

		L"idle_teen_left_2",
		L"idle_teen_right_2",

		L"idle_teen_left_3",
		L"idle_teen_right_3",

		L"idle_teen_sick_left_0",
		L"idle_teen_sick_right_0",

		L"run_teen_left_0",
		L"run_teen_right_0",

		L"walk_teen_back_0",
		L"walk_teen_front_0",

		L"walk_teen_frustrated_left_0",
		L"walk_teen_frustrated_right_0",
		L"walk_teen_frustrated_front_0",
	};
};

namespace NPC
{
	enum NPC_ENUM
	{
		IDLE_FRONT_0,
		IDLE_BACK_0,
		IDLE_LEFT_0,
		IDLE_RIGHT_0,

		END
	};

	constexpr int NPC_COUNT = END;

	std::array<std::wstring, NPC_COUNT> ANIMATIONS =
	{
		L"idle_front_0",
		L"idle_back_0",
		L"idle_left_0",
		L"idle_right_0",
	};
}
