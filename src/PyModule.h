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
extern CUtlVector<CGameEventListener*> g_vecEventListeners;


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

	//IGameEvent player_hurt
	//userid_pawn, attacker_pawn
	//GetPawnEHandle on above returns CEntityHandle
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

	m.def("GetZEPlayer", &CPlayerManager::GetPlayer);
	//ZEPlayer
	{
		py::class_<ZEPlayer*>(m, "ZEPlayer")
			.def("GetADVPlayer", 
				[](ZEPlayer& self) {
					return self.m_ADVPlayer;})
			;
			
	}

	//ADVPlayer
	{
		py::class_<ADVPlayer>(m, "ADVPlayer") //was ADVPlayer
			//-1    = invalid
			// 0..n = player of some kind
			.def(py::init<int>())

			.def("GetHealth", &ADVPlayer::GetHealth)
			.def("AddHealth", &ADVPlayer::AddHealth)
			.def("IsValid", &ADVPlayer::IsValid)
			.def("IsOnGround", &ADVPlayer::IsOnGround)
			.def("GetName", &ADVPlayer::GetName)
			.def("GetSlot", &ADVPlayer::GetSlot)
			.def("GetIndex", &ADVPlayer::GetIndex)
			.def("IsOnGround", &ADVPlayer::IsOnGround)
			.def("IsOnLadder", &ADVPlayer::IsOnLadder)
			.def("GetButtonStates", &ADVPlayer::GetButtonStates)
			.def("EmitSound", &ADVPlayer::EmitSound)
			.def("EmitSoundPersonal", &ADVPlayer::EmitSoundPersonal)
			.def("EmitSoundAll", &ADVPlayer::EmitSoundAll)
			.def_readwrite("_items_last_update", &ADVPlayer::_items_last_update)
			.def_readwrite("_last_inventory_change", &ADVPlayer::_last_inventory_change)
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
			// return const char*
			.def("GetName", &IGameEvent::GetName, "GetName() -> const char*")
			.def("GetString", &IGameEvent::GetString, "GetString(const GameEventKeySymbol_t, const char* defaultValue = \"\") -> const char*")

			// return int
			.def("GetID", &IGameEvent::GetID, "GetID() -> int")
			.def("GetInt", &IGameEvent::GetInt, "GetInt(const GameEventKeySymbol_t, int defaultValue = 0) -> int")

			// return bool
			.def("GetBool",    &IGameEvent::GetBool,    "GetBool(const GameEventKeySymbol_t, bool defaultValue = false) -> bool")
			.def("HasKey", &IGameEvent::HasKey, "HasKey(const GameEventKeySymbol_t) -> bool")
			.def("IsEmpty", &IGameEvent::IsEmpty, "GetEmpty(const GameEventKeySymbol_t) -> bool")
			.def("IsLocal",    &IGameEvent::IsLocal,    "GetLocal() -> bool")
			.def("IsReliable", &IGameEvent::IsReliable, "GetReliable() -> bool")

			// return float
			.def("GetFloat", &IGameEvent::GetFloat, "GetFloat(const GameEventKeySymbol_t, defaultValue = 0.0f) -> float")

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
			.def("GetPlayerSlot", &IGameEvent::GetPlayerSlot)

			// return CEntityInstance*
			.def("GetEntity", &IGameEvent::GetEntity)
			.def("GetPlayerController", &IGameEvent::GetPlayerController)
			.def("GetPlayerPawn", &IGameEvent::GetPlayerPawn)
			.def("GetEntity", &IGameEvent::GetEntity, py::return_value_policy::reference_internal)
			.def("GetPlayerController", &IGameEvent::GetPlayerController, py::return_value_policy::reference_internal) // returns CEntityInstance*
			.def("GetPlayerPawn", &IGameEvent::GetPlayerPawn, py::return_value_policy::reference_internal)			   // returns CEntityInstance*
		

			// return uint64
			.def("GetUint64", &IGameEvent::GetUint64)

			// return void*
			.def("GetPtr", &IGameEvent::GetPtr)
		;
	}
}