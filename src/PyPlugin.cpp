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
			Message("the loaded plugin: %s\n", m_PluginObject.str());
		}
		catch (py::error_already_set& e) {
			Message("modulename: %s\n", modulename.c_str());
			Message("Failed to load Python plugin: %s\n%s\n", filepath, std::string(e.what()));
			//Log::Error("Failed to load Python plugin: " + filepath + "\n" + std::string(e.what()));
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

	void PyPlugin::PyClientDisconnect(int playerSlot, int reason, const char* name, uint64_t xuid, const char* networkID)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientDisconnect", playerSlot, reason, name, xuid, networkID);
	}

	void PyPlugin::PyClientPutInServer(int playerSlot, char const* name, int type, uint64_t xuid)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientPutInServer", playerSlot, name, type, xuid);
	}

	void PyPlugin::PyClientSettingsChanged(int playerSlot)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientSettingsChanged", playerSlot);
	}

	void PyPlugin::PyOnClientConnected(int playerSlot, const char* name, uint64_t xuid, const char* networkID, const char* address, bool fakePlayer)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientConnected", playerSlot, name, xuid, networkID, address, fakePlayer);
	}

	void PyPlugin::PyClientConnect(int playerSlot, const char* name, uint64_t xuid, const char* networkID)
	{ 
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnClientConnect", playerSlot, name, xuid, networkID);
	}

	void PyPlugin::PyFireGameEvent(IGameEvent* event)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnGameEvent", event);
	}
	
	void PyPlugin::PyPlayerHurt(IGameEvent* event)
	{
		PyRuntime::ExecuteObjectMethod(m_PluginObject, "OnPlayerHurt", event);
	}

	/*
	//core.gameevents
	"player_hurt":	dict({
						"userid":			"playercontroller",
						"userid_pawn":		"strict_ehandle",
						"attacker":			"playercontroller",
						"attacker_pawn":	"strict_ehandle",
						"health":			"byte",
					}),
	
	//game.gameevents
	//N/A

	//mod.gameevents
	"player_hurt":	dict({
						"userid":			"playercontroller",	IGameEvent::GetPlayerController
						"userid_pawn":		"strict_ehandle",	IGameEvent::GetPawnEHandle
						"attacker":			"playercontroller",	IGameEvent::GetPlayerController
						"attacker_pawn":	"strict_ehandle",	IGameEvent::GetPawnEHandle	
						"health":			"byte",				IGameEvent::GetInt( "
						"armor":			"byte",
						"weapon":			"string",
						"dmg_health":		"short",			IGameEvent::GetInt("dmg_health", default=0)
						"dmg_armor":		"byte",				IGameEvent::GetInt("
						"hitgroup":			"byte",
					}),

	*/

}