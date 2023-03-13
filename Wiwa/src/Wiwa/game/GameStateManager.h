#pragma once

#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>
#include <Wiwa/ecs/components/game/Character.h>
#include <vector>

typedef size_t SceneId;

namespace Wiwa {

	enum class RoomType
	{
		NONE = -1,
		ROOM_HUB = 1,
		ROOM_COMBAT = 2,
		ROOM_REWARD = 3
	};

	enum class RoomState
	{
		NONE = -1,
		STATE_STARTING = 1,
		STATE_MIDDLE = 2,
		STATE_FINISHED = 3,
		STATE_AWAITING_NEXT = 4
	};

	class WI_API GameStateManager {
	private:
		static RoomType s_RoomType;
		static RoomState s_RoomState;
		static bool s_HasFinshedLevel;
		static bool s_CanPassNextLevel;
	public:

		// Save & Load Overall Player Progression
		void SaveProgression();
		void LoadProgression();

		// Save & Load Player data into player entity
		void SavePlayer(const Character& character);
		void LoadPlayer(Character& character);

		void QuerySpawners();
	};
}