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
