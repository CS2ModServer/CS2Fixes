#include "entity/ccsplayercontroller.h"
#include "entity/ccsplayerpawn.h"


class ADVAPI
{
public:
	
	bool SetHealthSlot(CPlayerSlot slot, int newHealth);
	bool SetHealthPawn(CCSPlayerPawn* pawn, int newHealth); //no way to get pawn yet
	bool SetHealthPlayerController(CCSPlayerController* ccsPC, int newHealth);

	int GetHealthSlot(CPlayerSlot slot);
	int GetHealthPawn(CCSPlayerPawn* pawn); //no way to get pawn yet
	int GetHealthPlayerController(CCSPlayerController* ccsPC);

	//CCSPlayerController* CEntityInstance_to_CCSPlayerController(CEntityInstance* ceInstance);
};