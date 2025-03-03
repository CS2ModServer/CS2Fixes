#include "entity/ccsplayercontroller.h"
#include "entity/ccsplayerpawn.h"
#include "playerslot.h"

class ADVAPI
{
public:
	ADVAPI(int _index) :
		index(_index) 
	{
		ccsPC = CCSPlayerController::FromSlot(index);
		pawn  = (CBaseEntity*)ccsPC->GetPawn();
	}

private:
	int index;
	CCSPlayerController* ccsPC;
	CBaseEntity* pawn;

public: 
	int						GetIndex() const	{ return index; }
	CCSPlayerController*	GetPC()				{ return ccsPC; }
	CBaseEntity*			GetPawn()			{ return pawn; }
	
	int GetHealth();
	void AddHealth(int amount);
	const char* GetName();
	bool IsValid();

};