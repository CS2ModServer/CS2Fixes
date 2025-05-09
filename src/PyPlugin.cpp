#include "PyPlugin.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace Source2Py {

	bool PyPlugin::LoadFromFile(std::string filepath) {
		std::string modulename = fs::path(filepath).stem().string();

		// Format filepath into module path for Python
		filepath.erase(filepath.find_last_of('.'), filepath.size());
		for (char& c : filepath) {
			if (c == '/' || c == '\\')
				c = '.';
		}

		// Attempt to load Python plugin
		try {
			m_PluginObject = py::module_::import(filepath.c_str()).attr(modulename.c_str());
			Message("Trying to load plugin: %s\n", modulename.c_str());
		}
		catch (py::error_already_set& e) {
			Message("Failed to load Python modulename: %s\n", modulename.c_str());
			Message("%s\n", e.what());
			m_Valid = false;
		}

		return m_Valid;
	}

	void PyPlugin::Load() {
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPluginLoad");
	}

	void PyPlugin::Unload() {
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPluginUnload");
	}

	void PyPlugin::PyGameFrame(bool simulating, bool firstTick, bool lastTick)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnGameFrame", simulating, firstTick, lastTick);
	}

	void PyPlugin::PyClientActive(int playerSlot, bool loadGame, const char* name, uint64_t xuid)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientActive", playerSlot, loadGame, name, xuid);
	}

	void PyPlugin::PyClientDisconnect(
		int playerSlot, 
		int reason, 
		const char* name, 
		uint64_t xuid, 
		const char* networkID)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientDisconnect", playerSlot, reason, name, xuid, networkID);
	}

	void PyPlugin::PyClientPutInServer(
		int playerSlot, 
		char const* name, 
		int type, 
		uint64_t xuid)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientPutInServer", playerSlot, name, type, xuid);
	}

	void PyPlugin::PyClientSettingsChanged(int playerSlot)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientSettingsChanged", playerSlot);
	}

	void PyPlugin::PyClientConnected(
		int playerSlot, 
		const char* name, 
		uint64_t xuid, 
		const char* networkID, 
		const char* address, 
		bool fakePlayer)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientConnected", playerSlot, name, xuid, networkID, address, fakePlayer);
	}

	void PyPlugin::PyFireGameEvent(IGameEvent* event)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnGameEvent", event);
	}
	
	void PyPlugin::PyPlayerHurt(IGameEvent* event)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerHurt", event);
	}

	void PyPlugin::PyPlayerDeath(IGameEvent* event)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerDeath", event);
	}

	void PyPlugin::PyPlayerDeathNew(
		IGameEvent* event,
		CPlayerSlot killerSlot,
		uint64_t killerSteam,
		CPlayerSlot victimSlot,
		uint64_t victimSteam,
		CPlayerSlot assisterSlot,
		uint64_t assisterSteam,
		bool suicide)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerDeath", event, killerSlot, killerSteam, victimSlot, victimSteam, assisterSlot, assisterSteam, suicide);
	}
	void PyPlugin::PyBombPlanted(IGameEvent* event, int slot, int site)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnBombPlanted", event, slot, site);
	}

	void PyPlugin::PyBombDefused(IGameEvent* event, int slot, int site)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnBombDefused", event, slot, site);
	}

	void PyPlugin::PyBombExploded(IGameEvent* event, int slot, int site)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnBombExploded", event, slot, site);
	}

	void PyPlugin::PyPlayerSpawn(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerSpawn", playerSlot);
	}

	void PyPlugin::PyPlayerSpawn_post(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerSpawn_post", playerSlot);
	}

	void PyPlugin::PyPlayerSpawned(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerSpawned", playerSlot);
	}

	void PyPlugin::PyPlayerActivate(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerActivate", playerSlot);
	}

	void PyPlugin::PyPlayerJump(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerJump", playerSlot);
	}

	void PyPlugin::PyPlayerLand(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerLand", playerSlot);
	}
	
	void PyPlugin::PyPlayerAirborn(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerAirborn", playerSlot);
	}

	void PyPlugin::PyClientCommand(int playerSlot, const char* command)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientCommand", playerSlot, command);
	}

	void PyPlugin::PyClientAbility1(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientAbility1", playerSlot);
	}

	void PyPlugin::PyClientAbility2(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientAbility2", playerSlot);
	}

	void PyPlugin::PyClientUltimate(int playerSlot)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientUltimate", playerSlot);
	}
	//void PyPlugin::PyMENU(auto& f)
	//{
	//	PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnMENU", f);
	//}
} // namespace Source2Py
