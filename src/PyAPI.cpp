#include "PyAPI.h"

#include <ISmmPlugin.h>
#include "PyInclude.h"
#include "commands.h"

extern IVEngineServer2* g_pEngineServer2;
extern ISmmAPI* g_SMAPI;
class CCSPlayerController;

namespace Source2Py
{

	void PyAPI::PrintToChat(int playerSlot, std::string message)
	{
		message.append("\n");
		CCSPlayerController* pc = CCSPlayerController::FromSlot(playerSlot);
		if (!pc || !pc->IsConnected() || pc->IsBot())
			return;

		ClientPrint(
			CCSPlayerController::FromSlot(
				CPlayerSlot(playerSlot)
				), 
			3, //see common.h for options, 3 is HUD_PRINTTALK
			message.c_str()
			);
	}
	void PyAPI::ConPrint(std::string message) {
		message.append("\n");
		META_CONPRINT(message.c_str()); 
	}

	void PyAPI::ClientConPrint(int playerSlot, std::string message) {
		message.append("\n");
		g_pEngineServer2->ClientPrintf(playerSlot, message.c_str());
	}

	void PyAPI::ServerCommand(const std::string& command) {
		g_pEngineServer2->ServerCommand(command.c_str());
	}

	void PyAPI::ClientCommand(int playerSlot, const std::string& command) {
		g_pEngineServer2->ClientCommand(playerSlot, command.c_str());
	}

	void PyAPI::SetTimescale(float timeScale)
	{
		g_pEngineServer2->SetTimescale(timeScale);
	}


	py::list PyAPI::GetPlayersNearCoords_list(py::dict vec, py::float_ distance, py::list ignore)
	{
		py::list l;
		Vector origin = Vector(
			vec["x"].cast<float>(), 
			vec["y"].cast<float>(), 
			vec["z"].cast<float>());

		for (int i = 0; i < GetGlobals()->maxClients; i++)
		{
			if (ignore.contains(i))
				continue;

			ZEPlayer* z = g_playerManager->GetPlayerFromUserId(i);
			if (!z)
				continue;
			
			ADVPlayer* ap = z->GetADVPlayer();
			if (!ap)
				continue;

			CBaseEntity* pawn = ap->GetPawn();
			if (!pawn)
				continue;

			Vector v = Vector(pawn->GetAbsOrigin());
			if (origin.DistTo(v) < distance.cast<float>())
			{
				l.append(i);
			}
		}
		return l;
	}

	py::list PyAPI::GetPlayersNearPlayerID_list(py::int_ playerid, py::float_ distance, py::list ignore)
	{
		py::list l;

		ZEPlayer* z = g_playerManager->GetPlayerFromUserId(playerid);
		if (!z)
			return l;
			
		ADVPlayer* ap = z->GetADVPlayer();
		if (!ap)
			return l;

		CBaseEntity* pawn = ap->GetPawn();
		if (!pawn)
			return l;

		Vector v = pawn->GetAbsOrigin();
		
		py::dict coords;
		coords["x"] = v.x;
		coords["y"] = v.y;
		coords["z"] = v.z;

		l = GetPlayersNearCoords_list(coords, distance, ignore);

		return l;
	}

	py::dict PyAPI::GetPlayersNearCoords_dict(py::dict vec, py::float_ distance, py::list ignore, bool return_difference)
	{
		py::dict d;
		Vector origin = Vector(
			vec["x"].cast<float>(),
			vec["y"].cast<float>(),
			vec["z"].cast<float>());

		for (int i = 0; i < GetGlobals()->maxClients; i++)
		{
			if (ignore.contains(i))
				continue;

			ZEPlayer* z = g_playerManager->GetPlayerFromUserId(i);
			if (!z)
				continue;

			ADVPlayer* ap = z->GetADVPlayer();
			if (!ap)
				continue;

			CBaseEntity* pawn = ap->GetPawn();
			if (!pawn)
				continue;

			Vector v = Vector(pawn->GetAbsOrigin());

			float difference = origin.DistTo(v);
			if (difference < distance.cast<float>())
			{
				if (return_difference)
				{
					d[py::int_(i)] = py::float_(difference);
				}
				else
				{
					py::dict coords;
					coords["x"] = v.x;
					coords["y"] = v.y;
					coords["z"] = v.z;

					d[py::int_(i)] = coords;
				}
			}
		}
		return d;
	}

	py::dict PyAPI::GetPlayersNearPlayerID_dict(py::int_ playerid, py::float_ distance, py::list ignore, bool return_difference)
	{
		py::dict d;

		ZEPlayer* z = g_playerManager->GetPlayerFromUserId(playerid);
		if (!z)
			return d;

		ADVPlayer* ap = z->GetADVPlayer();
		if (!ap)
			return d;

		CBaseEntity* pawn = ap->GetPawn();
		if (!pawn)
			return d;

		Vector v = pawn->GetAbsOrigin();

		py::dict coords;
		coords["x"] = v.x;
		coords["y"] = v.y;
		coords["z"] = v.z;

		d = GetPlayersNearCoords_dict(coords, distance, ignore, return_difference);

		return d;
	}


	/*int PyAPI::GetHealth(CEntityInstance* player)
	{
		CCSPlayerController* ccsPlayer = (CCSPlayerController*)player;
		if (!ccsPlayer)
			return -1;
		Message("PyAPI::GetHealth ccsPlayer true.\n");

		CCSPlayerPawn* ccsPawn = ccsPlayer->GetPlayerPawn();
		if (!ccsPawn)
			return -1;
		Message("PyAPI::GetHealth ccsPawn true.\n");
		return ccsPawn->m_iHealth();
	}*/

} // namespace Source2Py
