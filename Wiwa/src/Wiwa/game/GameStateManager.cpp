#include <wipch.h>
#include "GameStateManager.h"

namespace Wiwa
{
	RoomType GameStateManager::s_RoomType = RoomType::NONE;
	RoomState GameStateManager::s_RoomState = RoomState::NONE;
	bool GameStateManager::s_HasFinshedLevel = false;
	bool GameStateManager::s_CanPassNextLevel = false;

	void GameStateManager::SaveProgression()
	{
	}

	void GameStateManager::LoadProgression()
	{
	}

	void GameStateManager::SavePlayer(const Character& character)
	{
	}

	void GameStateManager::LoadPlayer(Character& character)
	{
	}
	void GameStateManager::QuerySpawners()
	{
	}
}


