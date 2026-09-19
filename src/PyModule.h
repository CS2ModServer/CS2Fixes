// Include this _once_ in a source file to define Source2Py module

#pragma once

#include "PyAPI.h"
#include "PyInclude.h"
#include "cs2fixes.h"

#include "adventuremod.h"

class CPlayerSlot;
#include <igameevents.h>
extern IGameEventManager2* g_gameEventManager;
#include "eventlistener.h"
//extern CUtlVector<CGameEventListener*> g_vecEventListeners;
extern std::vector<CGameEventListener*> g_vecEventListeners;


#include <pybind11/functional.h>


PYBIND11_EMBEDDED_MODULE(Source2Py, m) {
	using namespace Source2Py;

	m.def("ServerPrint", &PyAPI::ConPrint);
	m.def("ClientPrint", &PyAPI::ClientConPrint);

	m.def("ServerCommand", &PyAPI::ServerCommand);
	m.def("ClientCommand", &PyAPI::ClientCommand);

	m.def("SetTimescale", &PyAPI::SetTimescale);
	m.def("GetTickCount", 
		[]() { return GetGlobals()->tickcount; });

	//KeyValues3
	{
		py::enum_<KV3TypeEx_t>(m, "KV3TypeEx_t")
			.value("KV3_TYPEEX_INVALID", KV3TypeEx_t::KV3_TYPEEX_INVALID)
			.value("KV3_TYPEEX_NULL", KV3TypeEx_t::KV3_TYPEEX_NULL)
			;

		py::enum_<KV3SubType_t>(m, "KV3SubType_t")
			.value("KV3_TYPEEX_INVALID", KV3SubType_t::KV3_SUBTYPE_INVALID)
			.value("KV3_TYPEEX_NULL",    KV3SubType_t::KV3_SUBTYPE_UNSPECIFIED)
			;

		py::class_<KeyValues3>(m, "KeyValues3")
			.def(py::init(
				[](KV3TypeEx_t type, KV3SubType_t subtype) 
				{ return KeyValues3(type, subtype); }),
				py::arg("type") = KV3_TYPEEX_NULL,
				py::arg("subtype") = KV3_SUBTYPE_UNSPECIFIED )
			.def(py::init(
				[](int cluster_elem, KV3TypeEx_t type, KV3SubType_t subtype) 
				{ return KeyValues3(cluster_elem, type, subtype); }))
			;
	}

	//GameEventKeySymbol_t
	{
		py::class_<GameEventKeySymbol_t>(m, "GameEventKeySymbol_t")
			.def("GetString", &GameEventKeySymbol_t::GetString)
			.def(py::init<const char*>())
			.def(py::init([](py::str* s) 
			{ 
				//py::init<const char*>(((std::string*)s)->c_str());
				return GameEventKeySymbol_t(((std::string*)s)->c_str());
			}))
			;
	}

	//CEntityHandle
	{
		py::class_<CEntityHandle>(m, "CEntityHandle")
			.def(py::init<>())
			.def("Init", &CEntityHandle::Init, "Init(int iEntry, int iSerialNumber) -> void")
			.def("Term", &CEntityHandle::Term, "Term() -> void")
			.def("IsValid", &CEntityHandle::IsValid, "IsValid() -> bool")
			.def("GetEntryIndex", &CEntityHandle::GetEntryIndex, "GetEntryIndex() -> int")
			.def("GetSerialNumber", &CEntityHandle::GetSerialNumber, "GetSerialNumber() -> int")
			//.def("ToInt", &CEntityHandle::ToInt, "ToInt() -> int")
			.def(py::self != py::self, "-> bool")
			.def(py::self == py::self, "-> bool")
			.def(py::self < py::self, "-> bool")

			// just use Set
			//.def("=", &CEntityHandle::operator=, "-> const CEntityhandle&")
			.def("Set", &CEntityHandle::Set, "Set(const CEntityInstance* pEntity) -> const CEntityHandle&")
		
			// Use this to dereference the handle.
			// Note: this is implemented in game code (ehandle.h)
			.def("Get", &CEntityHandle::Get, "Get() -> CEntityInstance*")
			.def("TestSetHealth",
				[](CEntityHandle* self) 
				{
					Message("TestSetHealth - 1\n");
					if (!self->IsValid())
					{
						Message("TestSetHealth - IsValid=false\n");
					}
					else
					{
						CEntityInstance* instance = (CEntityInstance*)self->Get();

						Message("TestSetHealth - 2\n");
						CCSPlayerPawn* pawn = (CCSPlayerPawn*)instance;

						Message("TestSetHealth - 4\n");
						int hp = pawn->m_iHealth();
						 
						Message("TestSetHealth - 6\n");
						Message("hp: %d", hp);

						Message("TestSetHealth - 7\n");
					}
				})
		;
	}

	//CEntityInstance
	{
		py::class_<CEntityInstance, std::shared_ptr<CEntityInstance>>(m, "CEntityInstance")
			//.def(py::init<>()) // Doesn't have a constructor?
			.def("GetRefEHandle", &CEntityInstance::GetRefEHandle, "GetRefEHandle() -> CEntityHandle")
			.def("GetClassname", &CEntityInstance::GetClassname, "GetClassname() -> const char*")
			.def("GetEntityIndex", &CEntityInstance::GetEntityIndex, "GetEntityIndex() -> CEntityIndex")
			.def("GetHealth", 
				[](CEntityInstance* self) -> int 
				{
					CBaseEntity* pawn = (CBaseEntity*)self;
					if (!pawn)
						return -999;
					return (int)pawn->m_iHealth();
				})
			;
	}

	//CEntityIndex
	{
		py::class_<CEntityIndex>(m, "CEntityIndex")
			.def(py::init<int>())
			.def("Get", &CEntityIndex::Get, "Get() -> int")
			.def(py::self == py::self, "-> bool")
			.def(py::self != py::self, "-> bool")
			;
	}

	/*
	//CCommand
	{
		//went another route and not using.
		py::class_<CCommand>(m, "CCommand")
			.def("GetArgCount", &CCommand::ArgC, "() -> int")
			.def("GetMaxCommandLength", &CCommand::MaxCommandLength, "() -> int")
			.def("GetArgS", &CCommand::ArgS, "() -> const char*")
			.def("GetCommandString", &CCommand::GetCommandString, "() -> const char*")

			.def("GetArg", &CCommand::Arg, "(int index) -> const char*")

			.def("FindArg", &CCommand::FindArg, "(const char* pName) -> int")
			.def("FindArgInt", &CCommand::FindArgInt, "(const char* pName, int nDefaultVal) -> int")

			.def("__getitem__", &CCommand::operator[])
			//.def("GetArgV", &CCommand::ArgV) //idk what this one is atm.
			;
	}
	*/

	////ADVMenuAPI
	//{
	//	py::class_<ADVMenuAPI>(m, "ADVMenu")
	//		.def_readwrite("testmenu", &ADVMenuAPI::testname)
	//		;
	//}

	//ZEPlayer
	{
		m.def("GetZEPlayerOld", &CPlayerManager::GetPlayer);

		py::class_<ZEPlayer>(m, "ZEPlayer")
			.def("GetADVPlayerOld", 
				[](ZEPlayer& self) {
					return self.m_ADVPlayer;})
			.def("GetADVPlayer", &ZEPlayer::GetADVPlayer)
			;
			
	}

	//ADVPlayer
	{
		m.def("GetADVPlayer", 
			[](int slot) -> ADVPlayer* {
				  CPlayerSlot cpSlot = CPlayerSlot(slot);
				  ZEPlayer* zp = g_playerManager->GetPlayer(cpSlot);
				  return (ADVPlayer*)zp->GetADVPlayer();
			});
		m.def("GetZEPlayer",
			  [](int slot) -> ZEPlayer* {
				  CPlayerSlot cpSlot = CPlayerSlot(slot);
				  return (ZEPlayer*)g_playerManager->GetPlayer(cpSlot);
			  });
		py::class_<ADVPlayer>(m, "ADVPlayer") // was ADVPlayer
			//-1    = invalid
			// 0..n = player of some kind
			.def(py::init<int>())

			.def_property_readonly("playercontroller", &ADVPlayer::GetPC)
			.def("GetPC", &ADVPlayer::GetPC)
			.def_property_readonly("pawn", &ADVPlayer::GetPawn)
			.def("GetPawn", &ADVPlayer::GetPawn)
			.def_property_readonly("name", &ADVPlayer::GetName)
			.def("GetName", &ADVPlayer::GetName)

			.def_property("health",  &ADVPlayer::GetHealth, &ADVPlayer::SetHealth)
			.def("GetHealth", &ADVPlayer::GetHealth)
			.def("SetHealth", &ADVPlayer::SetHealth)
			.def("AddHealth", &ADVPlayer::AddHealth)

			.def("IsValid", &ADVPlayer::IsValid)

			.def_property_readonly("onground", &ADVPlayer::IsOnGround)
			.def("IsOnGround", &ADVPlayer::IsOnGround)
			.def_property_readonly("onladder", &ADVPlayer::IsOnLadder)
			.def("IsOnLadder", &ADVPlayer::IsOnLadder)
			
			.def("GetSlot", &ADVPlayer::GetSlot)
			.def("GetIndex", &ADVPlayer::GetIndex)
			.def("GetTeam", &ADVPlayer::GetTeam)
			.def("GetButtonStates", &ADVPlayer::GetButtonStates)
			.def("EmitSound", &ADVPlayer::EmitSound)
			.def("EmitSoundPersonal", &ADVPlayer::EmitSoundPersonal)
			.def("EmitSoundAll", &ADVPlayer::EmitSoundAll)
			.def_readwrite("_items_last_update", &ADVPlayer::_items_last_update)
			.def_readwrite("_last_inventory_change", &ADVPlayer::_last_inventory_change)
			.def_readwrite("_maptest_last_update", &ADVPlayer::_maptest_last_update)
			.def_readwrite("_maptest_last_class_list_change", &ADVPlayer::_maptest_last_class_list_change)
			.def_readwrite("_classes", &ADVPlayer::_classes)
			.def("test", 
				[](ADVPlayer& self) -> int 
				//Tristen, don't delete this, you'll eventually forget again.
				{
					return self.GetHealth();
				})
			;
	}

	m.def("CreateFakeEvent", 
		[](const char* name, bool bForce) -> IGameEvent*
		{
			
			IGameEvent* event = g_gameEventManager->CreateEvent(name,true);
			return event;
		},
		py::return_value_policy::reference_internal)
		;
	m.def("FireFakeEvent", 
		[](IGameEvent* event, bool bDontBroadcast) -> bool 
		{
			g_gameEventManager->FireEvent(event, bDontBroadcast);
			return true;
		});


	/*
	* Needed for IGameEvent if you want 
	* virtual CPlayerSlot GetPlayerSlot( const GameEventKeySymbol_t &keySymbol ) = 0;
	* virtual void SetPlayer( const GameEventKeySymbol_t &keySymbol, CPlayerSlot value ) = 0;
	*/
	//CPlayerSlot
	{
		py::class_<CPlayerSlot>(m, "CPlayerSlot")
			.def(py::init<int>())
			.def("Get", &CPlayerSlot::Get, "Get() -> int")
			.def(py::self == py::self, "-> bool")
			.def(py::self != py::self, "-> bool")

			.def("TestGetHealth",
				[](CPlayerSlot& self) -> int //this is overkill but at least it'll be understood when i come back some day.
				{	
					CBaseEntity* pawn = (CBaseEntity*)CCSPlayerController::FromSlot(self)->GetPawn();
					if (!pawn)
						return -999;

					return (int)pawn->m_iHealth();
				})
			.def("TestSetHealth",
				 [](CPlayerSlot& self, int newHealth) -> bool // this is overkill but at least it'll be understood when i come back some day.
				 {
					 CBaseEntity* pawn = (CBaseEntity*)CCSPlayerController::FromSlot(self)->GetPawn();
					 if (!pawn)
						 return false;

					 pawn->m_iHealth = newHealth;
					 return true;
				 })
			.def("TestAddHealth",
				[](CPlayerSlot& self, int addHealth) -> bool // this is overkill but at least it'll be understood when i come back some day.
				{
					CBaseEntity* pawn = (CBaseEntity*)CCSPlayerController::FromSlot(self)->GetPawn();
					if (!pawn)
						return false;

					pawn->m_iHealth = pawn->m_iHealth + addHealth;
					return true;
				})
			;
	}

	//CBasePlayerController
	{
		py::class_<CBasePlayerController*>(m, "CBasePlayerController")
			//.def("GetHealth", [](CBasePlayerController* self) { return self->m_iHealth(); }, "player health?")
			//.def("GetHealth", &CCSPlayerController::GetHealth, "GetHealth() -> int")
		;
	}

	//IGameEvent
	{
		py::class_<IGameEvent>(m, "GameEvent")
			
			// int
			.def_property("area", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("area"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("area"), value); })
			.def_property("assister", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("assister"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("assister"), value); })
			.def_property("attacker", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("attacker"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("attacker"), value); })
			.def_property("avenger_id", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("avenger_id"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("avenger_id"), value); })
			.def_property("avenged_player_id", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("avenged_player_id"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("avenged_player_id"), value); })
			.def_property("armor", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("armor"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("armor"), value); })
			.def_property("behavior", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("behavior"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("behavior"), value); })
			.def_property("damage_given", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("damage_given"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("damage_given"), value); })
			.def_property("damage_taken", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("damage_taken"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("damage_taken"), value); })
			.def_property("defindex", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("defindex"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("defindex"), value); })
			.def_property("dmg_armor", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("dmg_armor"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("dmg_armor"), value); })
			.def_property("dmg_health", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("dmg_health"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("dmg_health"), value); })
			.def_property("dominated", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("dominated"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("dominated"), value); })
			.def_property("entityid", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("entityid"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("entityid"), value); })
			.def_property("health", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("health"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("health"), value); })
			.def_property("hitgroup", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("hitgroup"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("hitgroup"), value); })
			.def_property("hits_given", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("hits_given"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("hits_given"), value); })
			.def_property("hits_taken", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("hits_taken"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("hits_taken"), value); })
			.def_property("hostage", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("hostage"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("hostage"), value); })
			.def_property("index", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("index"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("index"), value); })
			.def_property("killer", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("killer"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("killer"), value); })
			.def_property("legacy", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("legacy"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("legacy"), value); })
			.def_property("limit", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("limit"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("limit"), value); })
			.def_property("loadout", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("loadout"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("loadout"), value); })
			.def_property("newmode", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("newmode"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("newmode"), value); })
			.def_property("nomusic", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("nomusic"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("nomusic"), value); })
			.def_property("num_penetrations", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("num_penetrations"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("num_penetrations"), value); })
			.def_property("obs_target", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("obs_target"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("obs_target"), value); })
			.def_property("oldmode", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("oldmode"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("oldmode"), value); })
			.def_property("otherid", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("otherid"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("otherid"), value); })
			.def_property("penetrated", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("penetrated"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("penetrated"), value); })
			.def_property("player_count", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("player_count"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("player_count"), value); })
			.def_property("reason", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("reason"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("reason"), value); })
			.def_property("revenge", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("revenge"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("revenge"), value); })
			.def_property("site", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("site"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("site"), value); })
			.def_property("slot", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("slot"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("slot"), value); })
			.def_property("team", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("team"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("team"), value); })
			.def_property("userid", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("userid"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("userid"), value); })
			.def_property("victim", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("victim"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("victim"), value); })
			.def_property("weptype", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("weptype"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("weptype"), value); })
			.def_property("winner", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("winner"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("winner"), value); })
			.def_property("wipe", [](IGameEvent& self) -> py::int_ { return py::int_(self.GetInt(GameEventKeySymbol_t("wipe"), -1)); }, [](IGameEvent& self, int value) { self.SetInt(GameEventKeySymbol_t("wipe"), value); })
			
			// float
			.def_property("blind_duration", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("blind_duration"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("blind_duration"), value); })
			//player_falldamage event has "damage":"float", not added, maybe as "damage_float" but might not be needed.
			.def_property("distance", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("distance"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("distance"), value); })
			.def_property("damage_dir_x", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("damage_dir_x"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("damage_dir_x"), value); })
			.def_property("damage_dir_y", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("damage_dir_y"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("damage_dir_y"), value); })
			.def_property("damage_dir_z", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("damage_dir_z"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("damage_dir_z"), value); })
			.def_property("p", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("p"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("p"), value); })
			.def_property("r", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("r"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("r"), value); })
			.def_property("x", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("x"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("x"), value); })
			.def_property("y", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("y"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("y"), value); })
			.def_property("z", [](IGameEvent& self) -> py::float_ { return py::float_(self.GetFloat(GameEventKeySymbol_t("z"), -123456.7f)); }, [](IGameEvent& self, float value) { self.SetFloat(GameEventKeySymbol_t("z"), value); })

			// string
			.def_property("othertype", [](IGameEvent& self) -> py::str { return py::str(self.GetString(GameEventKeySymbol_t("othertype"), "unknown")); }, [](IGameEvent& self, py::str value) { self.SetString(GameEventKeySymbol_t("othertype"), std::string(value).c_str()); })
			.def_property("item", [](IGameEvent& self) -> py::str { return py::str(self.GetString(GameEventKeySymbol_t("item"), "unknown")); }, [](IGameEvent& self, py::str value) { self.SetString(GameEventKeySymbol_t("item"), std::string(value).c_str()); })
			.def_property("message", [](IGameEvent& self) -> py::str { return py::str(self.GetString(GameEventKeySymbol_t("message"), "unknown")); }, [](IGameEvent& self, py::str value) { self.SetString(GameEventKeySymbol_t("message"), std::string(value).c_str()); })
			.def_property("weapon", [](IGameEvent& self) -> py::str { return py::str(self.GetString(GameEventKeySymbol_t("weapon"), "unknown")); }, [](IGameEvent& self, py::str value) { self.SetString(GameEventKeySymbol_t("weapon"), std::string(value).c_str()); })
			.def_property("weapon_itemid", [](IGameEvent& self) -> py::str { return py::str(self.GetString(GameEventKeySymbol_t("weapon_itemid"), "unknown")); }, [](IGameEvent& self, py::str value) { self.SetString(GameEventKeySymbol_t("weapon_itemid"), std::string(value).c_str()); })
			.def_property("weapon_fauxitemid", [](IGameEvent& self) -> py::str { return py::str(self.GetString(GameEventKeySymbol_t("weapon_fauxitemid"), "unknown")); }, [](IGameEvent& self, py::str value) { self.SetString(GameEventKeySymbol_t("weapon_fauxitemid"), std::string(value).c_str()); })
			.def_property("weapon_originalowner_xuid", [](IGameEvent& self) -> py::str { return py::str(self.GetString(GameEventKeySymbol_t("weapon_originalowner_xuid"), "unknown")); }, [](IGameEvent& self, py::str value) { self.SetString(GameEventKeySymbol_t("weapon_originalowner_xuid"), std::string(value).c_str()); })

			// bool
			.def_property("assistedflash", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("assistedflash"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("assistedflash"), bool(value)); })
			.def_property("attackerblind", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("attackerblind"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("attackerblind"), bool(value)); })
			.def_property("attackerinair", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("attackerinair"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("attackerinair"), bool(value)); })
			.def_property("blocked", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("blocked"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("blocked"), bool(value)); })
			.def_property("canbuy", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("canbuy"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("canbuy"), bool(value)); })
			.def_property("canzoom", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("canzoom"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("canzoom"), bool(value)); })
			.def_property("enable", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("enable"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("enable"), bool(value)); })
			.def_property("hasbomb", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("hasbomb"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("hasbomb"), bool(value)); })
			.def_property("haskit", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("haskit"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("haskit"), bool(value)); })
			.def_property("hassilencer", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("hassilencer"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("hassilencer"), bool(value)); })
			.def_property("hastracers", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("hastracers"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("hastracers"), bool(value)); })
			.def_property("headshot", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("headshot"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("headshot"), bool(value)); })
			.def_property("in_air", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("in_air"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("in_air"), bool(value)); })
			.def_property("inrestart", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("inrestart"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("inrestart"), bool(value)); })
			.def_property("ispainted", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("ispainted"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("ispainted"), bool(value)); })
			.def_property("isplanted", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("isplanted"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("isplanted"), bool(value)); })
			.def_property("issilenced", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("issilenced"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("issilenced"), bool(value)); })
			.def_property("noreplay", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("noreplay"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("noreplay"), bool(value)); })
			.def_property("noscope", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("noscope"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("noscope"), bool(value)); })
			.def_property("no_scope", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("no_scope"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("no_scope"), bool(value)); })
			.def_property("silenced", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("silenced"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("silenced"), bool(value)); })
			.def_property("silent", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("silent"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("silent"), bool(value)); })
			.def_property("thrusmoke", [](IGameEvent& self) { return py::bool_(self.GetBool(GameEventKeySymbol_t("thrusmoke"), false)); }, [](IGameEvent& self, py::bool_ value) { self.SetBool(GameEventKeySymbol_t("thrusmoke"), bool(value)); })

			// return const char*
			.def("GetName", &IGameEvent::GetName, "GetName() -> const char*")
			.def("GetString", 
				[](IGameEvent& self, std::string key, std::string defaultValue) -> py::str 
				{ 
					return py::str(self.GetString(GameEventKeySymbol_t(key.c_str()), defaultValue.c_str())); 
				}, 
				py::arg("key") = std::string("Unknown"), 
				py::arg("defaultValue") = std::string("Unknown")
				)

			// return int
			.def("GetID", &IGameEvent::GetID)
			.def("GetInt",
				 [](IGameEvent& self, std::string key, int defaultValue) -> int
				 {
					return self.GetInt(GameEventKeySymbol_t(key.c_str()), defaultValue);
				 },
				 py::arg("key"), 
				 py::arg("defaultValue") = 0
				 )

			// return uint64
			.def("GetUint64", &IGameEvent::GetUint64)

			// return bool
			.def("GetBool", 
				[](IGameEvent& self, std::string key, bool defaultValue) -> bool 
				{ return self.GetBool(GameEventKeySymbol_t(key.c_str()), defaultValue); }, 
				py::arg("key"), 
				py::arg("defaultValue") = false
				)
			.def("HasKey", 
				[](IGameEvent& self, std::string key) -> bool 
				{ return self.HasKey(GameEventKeySymbol_t(key.c_str())); })
			.def("IsEmpty", [](IGameEvent& self, std::string key) -> bool 
				{ return self.IsEmpty(GameEventKeySymbol_t(key.c_str())); })
			.def("IsLocal",    &IGameEvent::IsLocal,    "GetLocal() -> bool")
			.def("IsReliable", &IGameEvent::IsReliable, "GetReliable() -> bool")

			// return float
			.def("GetFloat", 
				[](IGameEvent& self, std::string key, float defaultValue) -> float 
				{ return self.GetFloat(GameEventKeySymbol_t(key.c_str()), defaultValue); }, 
				py::arg("key"), 
				py::arg("defaultValue") = 0.0f
				)

			// return KeyValues3*
			.def("GetDataKeys", &IGameEvent::GetDataKeys, "GetDataKeys() -> KeyValues3*")
		
			// void no return
			.def("SetBool", &IGameEvent::SetBool)
			.def("SetInt", &IGameEvent::SetInt)
			.def("SetUint64", &IGameEvent::SetUint64)
			.def("SetFloat", &IGameEvent::SetFloat)
			.def("SetString", &IGameEvent::SetString)
			.def("SetPtr", &IGameEvent::SetPtr)
			.def("SetPlayerRaw", &IGameEvent::SetPlayerRaw)
			//.def("SetPlayer", &IGameEvent::SetPlayer) //figure out overloading
			//.def("SetEntity", &IGameEvent::SetEntity) //figure out overloading
			//.def("unk001", &IGameEvent::unk001) //virtual void* unk001() = 0; //Something script vm related

			// return CEntityHandle
			.def("GetEHandle", &IGameEvent::GetEHandle)
			.def("GetPawnEHandle", &IGameEvent::GetPawnEHandle)

			// return CEntityIndex
			.def("GetEntityIndex", &IGameEvent::GetEntityIndex)
			.def("GetPawnEntityIndex", &IGameEvent::GetPawnEntityIndex)

			// return CPlayerSlot
			.def_property_readonly("playerslot", 
				[](IGameEvent& self, std::string key) -> CPlayerSlot 
				{ return self.GetPlayerSlot(GameEventKeySymbol_t(key.c_str())); })
			.def("GetPlayerSlot", 
				[](IGameEvent& self, std::string key) -> CPlayerSlot 
				{ return self.GetPlayerSlot(GameEventKeySymbol_t(key.c_str())); })

			// return CEntityInstance*
			.def_property_readonly("entity", 
				[](IGameEvent& self, std::string key) -> CEntityInstance* 
				{ return self.GetEntity(GameEventKeySymbol_t(key.c_str())); },
				py::return_value_policy::reference_internal) 
			.def("GetEntity", 
				[](IGameEvent& self, std::string key) -> CEntityInstance* 
				{ return self.GetEntity(GameEventKeySymbol_t(key.c_str())); },
				py::return_value_policy::reference_internal) 
			.def_property_readonly("playercontroller",
				[](IGameEvent& self, std::string key) -> CEntityInstance*
				{ return self.GetPlayerController(GameEventKeySymbol_t(key.c_str())); },
				py::return_value_policy::reference_internal)
			.def("GetPlayerController",
				[](IGameEvent& self, std::string key) -> CEntityInstance*
				{ return self.GetPlayerController(GameEventKeySymbol_t(key.c_str())); },
				py::return_value_policy::reference_internal)
			.def_property_readonly("playerpawn", 
				[](IGameEvent& self, std::string key) -> CEntityInstance*
				{ return self.GetPlayerPawn(GameEventKeySymbol_t(key.c_str())); },
				py::return_value_policy::reference_internal) 
			.def("GetPlayerPawn", 
				[](IGameEvent& self, std::string key) -> CEntityInstance*
				{ return self.GetPlayerPawn(GameEventKeySymbol_t(key.c_str())); },
				py::return_value_policy::reference_internal) 

			// return void*
			.def("GetPtr", 
				[](IGameEvent& self, std::string key) -> void* 
				{ return self.GetPtr(GameEventKeySymbol_t(key.c_str())); },
				py::return_value_policy::reference_internal)
		;
	}
}