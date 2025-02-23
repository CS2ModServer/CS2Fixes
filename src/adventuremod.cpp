#include "adventuremod.h"

//SET HEALTH
bool ADVAPI::SetHealthSlot(CPlayerSlot slot, int newHealth)
{
	if (slot.Get() <= 0)
		return false;

	CCSPlayerController* ccsPC = CCSPlayerController::FromSlot(slot);
	ccsPC->m_iHealth = newHealth;
	return true;
}

bool ADVAPI::SetHealthPawn(CCSPlayerPawn* pawn, int newHealth)
{
	if (!pawn)
		return false;

	CCSPlayerController* ccsPC = CCSPlayerController::FromPawn(pawn);
	if (!ccsPC)
		return false;

	ccsPC->m_iHealth = newHealth;
	return true;
}

bool ADVAPI::SetHealthPlayerController(CCSPlayerController* ccsPC, int newHealth)
{
	if (!ccsPC)
		return false;

	ccsPC->m_iHealth = newHealth;
	return true;
}

// GET HEALTH
int ADVAPI::GetHealthSlot(CPlayerSlot slot)
{
	if (slot.Get() <= 0)
		return -999;

	CCSPlayerController* ccsPC = CCSPlayerController::FromSlot(slot);
	if (!ccsPC)
		return -999;

	return ccsPC->m_iHealth();
}

int ADVAPI::GetHealthPawn(CCSPlayerPawn* pawn)
{
	if (!pawn)
		return -999;

	CCSPlayerController* ccsPC = CCSPlayerController::FromPawn(pawn);
	if (!ccsPC)
		return -999;

	return ccsPC->m_iHealth();
}

int ADVAPI::GetHealthPlayerController(CCSPlayerController* ccsPC)
{
	if (!ccsPC)
		return -999;

	return ccsPC->m_iHealth();
}

//CCSPlayerController* ADVAPI::CEntityInstance_to_CCSPlayerController(CEntityInstance* ceInstance)
//{
//	CCSPlayerController* ccsPlayer = CCSPlayerController::FromPawn(static_cast<CCSPlayerPawn*>(ceInstance));
//	return ccsPlayer;
//}
