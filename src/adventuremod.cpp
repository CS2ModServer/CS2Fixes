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
	return ent->m_fFlags() & FL_ONGROUND;
}

bool ADVAPI::IsOnLadder()
{
	CBaseEntity* ent = (CBaseEntity*)GetPawn();
	return ent->m_MoveType() & MOVETYPE_LADDER;
}
