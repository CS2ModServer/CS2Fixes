#include "adventuremod.h"

#include "entity/ccsplayercontroller.h"
#include "entity/ccsplayerpawn.h"
#include "playerslot.h"
#include "common.h"


#include "commands.h"
#include "cs2fixes.h"
#include "recipientfilters.h"

int ADVPlayer::GetHealth()
{
	CBaseEntity* pawn = GetPawn();
	if (pawn && pawn->IsAlive())
		return pawn->m_iHealth();

	return -999;
}

void ADVPlayer::AddHealth(int amount)
{
	CBaseEntity* pawn = GetPawn();
	if (pawn && pawn->IsAlive())
	{
		pawn->m_iHealth = pawn->m_iHealth() + amount;
	}
}

const char* ADVPlayer::GetName()
{
	CCSPlayerController* pc = GetPC();
	if (!pc)
		return nullptr;

	return pc->GetPlayerName();
}

bool ADVPlayer::IsValid()
{
	if (this->GetIndex() < 0)
		return false;

	CCSPlayerController* pc = this->GetPC();
	if (!pc)
		return false;

	CBaseEntity* pawn = pc->GetPawn();
	if (!pawn)
		return false;

	return true;
}

bool ADVPlayer::IsOnGround()
{
	CBaseEntity* ent = (CBaseEntity*)GetPawn();
	if (ent)
		return ent->m_fFlags() & FL_ONGROUND;
	return false;
}

bool ADVPlayer::IsOnLadder()
{
	CBaseEntity* ent = (CBaseEntity*)GetPawn();
	if (ent)
		return ent->m_MoveType() & MOVETYPE_LADDER;
	return false;
}

uint64* ADVPlayer::GetButtonStates()
{
	CBaseEntity* pawn = (CBaseEntity*)GetPawn();
	CCSPlayerPawnBase* base = (CCSPlayerPawnBase*)pawn;
	CPlayer_MovementServices* ms = base->m_pMovementServices();
	// m_pButtonStates[0] is the mask of currently pressed buttons
	// m_pButtonStates[1] is the mask of buttons that changed in the current frame
	
	//uint64* temp = ms->m_nButtons().m_pButtonStates();
	//return temp;
	
	return ms->m_nButtons().m_pButtonStates(); 
}

bool ADVPlayer::EmitSound(
	const char* _pszSound, 
	int _nPitch=100, 
	float _flVolume = 1.0f, 
	float _flDelay = 0.0f)
{
	CCSPlayerController* pc = GetPC();
	if (!pc)
		return false;

	CCSPlayerPawn* ent = (CCSPlayerPawn*)pc->GetPawn();
	if (!ent)
		return false;

	ent->EmitSound(_pszSound, _nPitch, _flVolume, _flDelay);
	return true;
}

bool ADVPlayer::EmitSoundPersonal(
	const char* _pszSound,
	int _nPitch = 100,
	float _flVolume = 1.0f)
{
	CSingleRecipientFilter _filter = CSingleRecipientFilter(GetSlot());

	CCSPlayerController* pc = GetPC();
	if (!pc)
		return false;

	CCSPlayerPawn* ent = (CCSPlayerPawn*)pc->GetPawn();
	if (!ent)
		return false;

	ent->EmitSoundFilter(_filter, _pszSound, _nPitch, _flVolume);
	return true;
	}

bool ADVPlayer::EmitSoundAll(
	const char* _pszSound,
	int _nPitch = 100,
	float _flVolume = 1.0f)
{
	CRecipientFilter _filter = CRecipientFilter();
	_filter.AddAllPlayers();

	CCSPlayerController* pc = GetPC();
	if (!pc)
		return false;

	CCSPlayerPawn* ent = (CCSPlayerPawn*)pc->GetPawn();
	if (!ent)
		return false;

	ent->EmitSoundFilter(_filter, _pszSound, _nPitch, _flVolume);
	return true;
	}


CON_COMMAND_F(ability1, "Technique bound to ability1", FCVAR_NONE)
{
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyClientAbility1(context.GetPlayerSlot().Get());
	return;
}
CON_COMMAND_F(ability2, "Technique bound to ability2", FCVAR_NONE)
{
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyClientAbility2(context.GetPlayerSlot().Get());
	return;
}
CON_COMMAND_F(ultimate, "Technique bound to ultimate", FCVAR_NONE)
{
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyClientUltimate(context.GetPlayerSlot().Get());
	return;
}
// return a list of the players current items.
std::vector<std::string> ADVPlayer::GetPlayerItems()
{
	// test changing an outer std::string //also works
	if (GetLastUpdateTime() < GetLastInventoryChange())
		UpdatePlayerItems();

	return *_items;
}

int ADVPlayer::GetLastUpdateTime()
{
	return _items_last_update;
}
int ADVPlayer::GetLastInventoryChange()
{
	return _last_inventory_change;
}

bool ADVPlayer::UpdatePlayerItems()
{
	_items_last_update = GetGlobals()->tickcount;
	// outside
	std::string outerstdstring = "default";

	// lambda
	std::function<bool(py::str&)> _teststdstring(
		[&outerstdstring](py::str& innerstdstring) {
			Message("cs2fixes templatefunc - START\n");
			outerstdstring = innerstdstring;
			Message("cs2fixes templatefunc - END\n");
			return true;
		});

	// tell python plugins to use lambda
	for (auto& plugin : g_CS2Fixes.m_Plugins)
	{
		Message("cs2fixes outerstdstring before: %s\n", outerstdstring);
		py::object object = plugin.GetSelf();
		bool success = Source2Py::PyRuntime::ExecuteObjectMethod(object, "_teststdstring", _teststdstring);
		Message("success=%d\n", success);
		Message("cs2fixes outerstdstring  after: %s\n\n\n", outerstdstring);
	}

	_items[index].clear();
	_items[index].push_back(outerstdstring);
	_items_last_update = GetGlobals()->tickcount;
	return true;
}

CCSPlayerController* ADVPlayer::GetPC()
{
	return (CCSPlayerController*)CCSPlayerController::FromSlot(CPlayerSlot(index));
}

CBaseEntity* ADVPlayer::GetPawn()
{
	CCSPlayerController* pc = GetPC();
	if (pc)
		return (CBaseEntity*)pc->GetPawn();
	else
		return nullptr;
}
