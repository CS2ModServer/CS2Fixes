#include "adventuremod.h"

int ADVAPI::GetHealth()
{
	CBaseEntity* pawn = GetPawn();
	if (pawn && pawn->IsAlive())
		return pawn->m_iHealth();

	return -999;
}

void ADVAPI::AddHealth(int amount)
{
	CBaseEntity* pawn = GetPawn();
	if (pawn && pawn->IsAlive())
	{
		pawn->m_iHealth = pawn->m_iHealth() + amount;
	}
}

const char* ADVAPI::GetName()
{
	CCSPlayerController* pc = GetPC();
	if (!pc)
		return nullptr;

	return pc->GetPlayerName();
}

bool ADVAPI::IsValid()
{
	if (this->GetIndex() < 0)
		return false;

	return true;
}

bool ADVAPI::IsOnGround()
{
	CBaseEntity* ent = (CBaseEntity*)GetPawn();
	if (ent)
		return ent->m_fFlags() & FL_ONGROUND;
	return false;
}

bool ADVAPI::IsOnLadder()
{
	CBaseEntity* ent = (CBaseEntity*)GetPawn();
	if (ent)
		return ent->m_MoveType() & MOVETYPE_LADDER;
	return false;
}

uint64* ADVAPI::GetButtons()
{
	CBaseEntity* pawn = (CBaseEntity*)GetPawn();
	CCSPlayerPawnBase* base = (CCSPlayerPawnBase*)pawn;
	CPlayer_MovementServices* ms = base->m_pMovementServices();
	// m_pButtonStates[0] is the mask of currently pressed buttons
	// m_pButtonStates[1] is the mask of buttons that changed in the current frame
	uint64* temp = ms->m_nButtons().m_pButtonStates();
	return temp;
}