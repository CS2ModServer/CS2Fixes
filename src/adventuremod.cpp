#include "adventuremod.h"

int ADVAPI::GetHealth()
{
	if (pawn)
		return pawn->m_iHealth();

	return -999;
}

void ADVAPI::AddHealth(int amount)
{
	if (pawn)
		pawn->m_iHealth = pawn->m_iHealth() + amount;
}

const char* ADVAPI::GetName()
{
	if (!ccsPC)
		return nullptr;

	return ccsPC->GetPlayerName();
}

bool ADVAPI::IsValid()
{
	//todo is zero an acceptable player index?
	if (this->GetIndex() < 0)
		return false;

	if (!this->GetPC())
		return false;

	if (!this->GetPawn())
		return false;

	return true;
}
