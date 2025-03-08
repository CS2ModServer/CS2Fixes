#include "entity/ccsplayercontroller.h"
#include "entity/ccsplayerpawn.h"
#include "playerslot.h"

class ADVAPI
{
public:
	ADVAPI(int _index) :
		index(_index) 
	{
	}

private:
	int index;

public: 
	int						GetIndex() const	{ return index; }
	int						GetSlot() const		{ return index; }
	CCSPlayerController*	GetPC() 
	{ 
		return (CCSPlayerController*)CCSPlayerController::FromSlot(index); 
	}
	CBaseEntity*			GetPawn()			
	{ 
		CCSPlayerController* pc = GetPC();
		return (CBaseEntity*)pc->GetPawn();
	}
	
	int GetHealth();
	void AddHealth(int amount);
	const char* GetName();
	bool IsValid();
	bool IsOnGround();
	bool IsOnLadder();




};