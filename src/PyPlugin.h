#pragma once

#include "PyRuntime.h"
#include "convar.h"

#include <igameevents.h>

namespace Source2Py {

	class PyPlugin {
	public:
		PyPlugin() = default;
		PyPlugin(std::string filepath) { this->LoadFromFile(filepath); }

		bool LoadFromFile(std::string filepath);

		// Python plugin methods
		void Load();
		void Unload();

		// Python plugin hooks
		void PyClientActive(int playerSlot, bool loadGame, const char* name, uint64_t xuid);
		void PyClientDisconnect(int playerSlot, int reason, const char* name, uint64_t xuid, const char* networkID);
		void PyClientConnected(int playerSlot, const char* name, uint64_t xuid, const char* networkID, const char* address, bool fakePlayer);
		void PyClientPutInServer(int playerSlot, char const* name, int type, uint64_t xuid);
		void PyClientSettingsChanged(int playerSlot);
		void PyFireGameEvent(IGameEvent* event);
		void PyGameFrame(bool simulating, bool firstTick, bool lastTick);
		void PyPlayerActivate(int playerSlot);
		
		void PyClientCommand(int playerSlot, const char* command);
		void PyClientAbility1(int playerSlot);
		void PyClientAbility2(int playerSlot);
		void PyClientUltimate(int playerSlot);
		
		void PyPlayerHurt(IGameEvent* event);
		void PyPlayerDeath(IGameEvent* event);
		
		void PyPlayerJump(int playerSlot);
		void PyPlayerAirborn(int playerSlot);
		void PyPlayerLand(int playerSlot);

		void PyPlayerSpawn(int playerSlot);
		void PyPlayerSpawn_post(int playerSlot);
		void PyPlayerSpawned(int playerSlot);


		bool IsValid() const { return m_Valid; }

		operator bool() { return m_Valid; }

	private:
		py::object m_PluginObject;
		bool m_Valid = true;
	};
}