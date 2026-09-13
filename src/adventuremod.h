#pragma once

//#include "entity/ccsplayercontroller.h"
//#include "entity/ccsplayerpawn.h"
//#include "playerslot.h"
//
//#include "common.h"
#include <string>
#include <vector>
#include <platform.h>
#include <map>
#include "PyInclude.h"
//#include "adventuremenu.h"

#define MAXITEMS 50

class CCSPlayerController;
class CBaseEntity;

class ADVPlayer
{
public:
	ADVPlayer()
	{
		index = -1;
		_items_last_update = -1;
		_last_inventory_change = -1;
		_maptest_last_update = -1;
		_maptest_last_class_list_change = -1;
	};

	ADVPlayer(int _index) :
		index(_index)
	{
		_items_last_update = -1;
		_last_inventory_change = -1;
		_maptest_last_update = -1;
		_maptest_last_class_list_change = -1;
		UpdatePlayerItems();
		_maptest_UpdatePlayerClassList();
	};

private:
	int index;

public: 
	int						GetIndex() const	{ return index; };
	int						GetSlot() const		{ return index; };
	int						GetTeam();

	CCSPlayerController*    GetPC();
	CBaseEntity*			GetPawn();
	int GetHealth();
	void AddHealth(int amount);
	const char* GetName();
	bool IsValid();
	bool IsOnGround();
	bool IsOnLadder();
	uint64* GetButtonStates();
	bool EmitSound(
		const char* _pszSound, 
		int   _nPitch, 
		float _flVolume, 
		float _flDelay);
	bool EmitSoundPersonal(
		const char* _pszSound,
		int _nPitch,
		float _flVolume);
	bool EmitSoundAll(
		const char* _pszSound,
		int _nPitch,
		float _flVolume);

	
	// chr menu interaction
private: 
	//vector of item names the player has in python.
	std::vector<std::string> _items[MAXITEMS];

public:

	// the last time the _players_items list was updated.
	int _items_last_update;
	int _last_inventory_change;

	// return a list of the players current items.
	std::vector<std::string> GetPlayerItems();

	//get a new list of items from python.
	bool UpdatePlayerItems();
	int GetLastUpdateTime();
	int GetLastInventoryChange();

public:

	int _maptest_last_update;
	int _maptest_last_class_list_change;
	//py::list _classes[MAXITEMS];
	py::list _classes;

	//std::map<std::string, int> _maptest_GetPlayerClasses(int slot, int menutype);
	py::list _maptest_GetPlayerClasses();

	bool _maptest_UpdatePlayerClassList();
	int _maptest_GetLastUpdateTime();
	int _maptest_GetLastClassListChange();
};

