/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2025 Source2ZE
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "KeyValues.h"
#include "commands.h"
#include "common.h"
#include "ctimer.h"
#include "entities.h"
#include "entity/cbaseplayercontroller.h"
#include "entity/cgamerules.h"
#include "entwatch.h"
#include "eventlistener.h"
#include "idlemanager.h"
#include "leader.h"
#include "map_votes.h"
#include "networkstringtabledefs.h"
#include "panoramavote.h"
#include "recipientfilters.h"
#include "votemanager.h"
#include "zombiereborn.h"
#include "adventuremod.h"

#include "cs2fixes.h"

#include "tier0/memdbgon.h"

extern IGameEventManager2* g_gameEventManager;
extern IServerGameClients* g_pSource2GameClients;
extern CGameEntitySystem* g_pEntitySystem;
extern CGlobalVars* GetGlobals();
extern CCSGameRules* g_pGameRules;
extern IVEngineServer2* g_pEngineServer2;

extern int g_iRoundNum;

CUtlVector<CGameEventListener*> g_vecEventListeners;

void RegisterEventListeners()
{
	static bool bRegistered = false;

	if (bRegistered || !g_gameEventManager)
		return;
	
	//E:/CS2Server/cs2/game/csgo/bin/win64/~ is where were at normally.
	g_gameEventManager->LoadEventsFromFile("../../csgo/addons/CS2Fixes/resource/adventure.res", false);

	FOR_EACH_VEC(g_vecEventListeners, i)
	{
		g_gameEventManager->AddListener(g_vecEventListeners[i], g_vecEventListeners[i]->GetEventName(), true);
	}

	bRegistered = true;
}

void UnregisterEventListeners()
{
	if (!g_gameEventManager)
		return;

	FOR_EACH_VEC(g_vecEventListeners, i)
	{
		g_gameEventManager->RemoveListener(g_vecEventListeners[i]);
	}

	g_vecEventListeners.Purge();
}

bool g_bPurgeEntityNames = false;
FAKE_BOOL_CVAR(cs2f_purge_entity_strings, "Whether to purge the EntityNames stringtable on new rounds", g_bPurgeEntityNames, false, false);

extern void FullUpdateAllClients();

GAME_EVENT_F(round_prestart)
{
	g_iRoundNum++;

	if (g_bPurgeEntityNames)
	{
		INetworkStringTable* pEntityNames = g_pNetworkStringTableServer->FindTable("EntityNames");

		if (pEntityNames)
		{
			int iStringCount = pEntityNames->GetNumStrings();
			addresses::CNetworkStringTable_DeleteAllStrings(pEntityNames);

			Message("Purged %i strings from EntityNames\n", iStringCount);

			// Vauff: Not fixing cubemap fog in my testing
			// This also breaks round start particle resets, so disabling for now
			// pEntityNames->SetTick(-1, nullptr);

			// FullUpdateAllClients();
		}
	}

	EntityHandler_OnRoundRestart();

	CBaseEntity* pShake = nullptr;

	// Prevent shakes carrying over from previous rounds
	while ((pShake = UTIL_FindEntityByClassname(pShake, "env_shake")))
		pShake->AcceptInput("StopShake");

	if (g_bEnableZR)
		ZR_OnRoundPrestart(pEvent);

	if (g_bEnableEntWatch)
		EW_RoundPreStart();
}

static bool g_bBlockTeamMessages = false;

FAKE_BOOL_CVAR(cs2f_block_team_messages, "Whether to block team join messages", g_bBlockTeamMessages, false, false)

GAME_EVENT_F(player_team)
{
	// Remove chat message for team changes
	if (g_bBlockTeamMessages)
		pEvent->SetBool("silent", true);
}

static bool g_bNoblock = false;

FAKE_BOOL_CVAR(cs2f_noblock_enable, "Whether to use player noblock, which sets debris collision on every player", g_bNoblock, false, false)

GAME_EVENT_F(player_spawned)
{
	int index = pEvent->GetPlayerSlot("userid").Get();
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerSpawned(index);
	
	return;
}

GAME_EVENT_F(player_activate)
{
	int index = pEvent->GetPlayerSlot("userid").Get();
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerActivate(index);
	
	return;
}

GAME_EVENT_F(player_spawn)
{
	int index = pEvent->GetPlayerSlot("userid").Get();
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerSpawn(index);

	CCSPlayerController* pController = (CCSPlayerController*)pEvent->GetPlayerController("userid");
	
	if (!pController)
		return;

	ZEPlayer* pPlayer = pController->GetZEPlayer();

	// always reset when player spawns
	if (pPlayer)
		pPlayer->SetMaxSpeed(1.f);

	if (g_bEnableZR)
		ZR_OnPlayerSpawn(pController);

	if (pController->IsConnected())
		pController->GetZEPlayer()->OnSpawn();

	CHandle<CCSPlayerController> hController = pController->GetHandle();

	// Gotta do this on the next frame...
	new CTimer(0.0f, false, false, [hController]() {
		CCSPlayerController* pController = hController.Get();

		int index = pController->GetPlayerSlot();
		for (auto& plugin : g_CS2Fixes.m_Plugins)
			plugin.PyPlayerSpawn_post(index);

		if (!pController)
			return -1.0f;

		if (const auto player = pController->GetZEPlayer())
			player->SetSteamIdAttribute();

		if (!pController->m_bPawnIsAlive())
			return -1.0f;

		CBasePlayerPawn* pPawn = pController->GetPawn();

		// Just in case somehow there's health but the player is, say, an observer
		if (!g_bNoblock || !pPawn || !pPawn->IsAlive())
			return -1.0f;

		pPawn->SetCollisionGroup(COLLISION_GROUP_DEBRIS);

		return -1.0f;
	});

	// And this needs even more delay..? Don't even know if this is enough, bug can't be reproduced
	new CTimer(0.1f, false, false, [hController]() {
		CCSPlayerController* pController = hController.Get();

		if (!pController)
			return -1.0f;

		CBasePlayerPawn* pPawn = pController->GetPawn();

		if (pPawn)
		{
			// Fix new haunted CS2 bug? https://www.reddit.com/r/cs2/comments/1glvg9s/thank_you_for_choosing_anubis_airlines/
			// We've seen this several times across different maps at this point
			pPawn->m_vecAbsVelocity = Vector(0, 0, 0);
		}

		return -1.0f;
	});

}

static bool g_bEnableTopDefender = false;

FAKE_BOOL_CVAR(cs2f_topdefender_enable, "Whether to use TopDefender", g_bEnableTopDefender, false, false)

/*
//core.gameevents
"player_hurt":	dict({
					"userid":			"playercontroller",
					"userid_pawn":		"strict_ehandle",
					"attacker":			"playercontroller",
					"attacker_pawn":	"strict_ehandle",
					"health":			"byte",
				}),
//mod.gameevents
"player_hurt":	dict({
					"userid":			"playercontroller",
					"userid_pawn":		"strict_ehandle",
					"attacker":			"playercontroller",
					"attacker_pawn":	"strict_ehandle",
					"health":			"byte",
					"armor":			"byte",
					"weapon":			"string",
					"dmg_health":		"short",
					"dmg_armor":		"byte",
					"hitgroup":			"byte",
				}),

*/

//remember this is firing at the moment the player is hurt, before the damage is applied.
GAME_EVENT_F(player_hurt) //new
{
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerHurt(pEvent);

	//if (g_bEnableZR)
	//	ZR_OnPlayerHurt(pEvent);

	//if (!g_bEnableTopDefender)
	//	return;

	CCSPlayerController* pAttacker = (CCSPlayerController*)pEvent->GetPlayerController("attacker");
	CCSPlayerController* pVictim = (CCSPlayerController*)pEvent->GetPlayerController("userid");
	
	//pEvent->SetInt("attacker_slot", pAttacker->GetPlayerSlot());
	//pEvent->SetInt("victim_slot", pVictim->GetPlayerSlot());

	// Ignore Ts/zombies and CTs hurting themselves
	if (!pAttacker || pAttacker->m_iTeamNum() != CS_TEAM_CT || pAttacker->m_iTeamNum() == pVictim->m_iTeamNum())
		return;

	ZEPlayer* pPlayer = pAttacker->GetZEPlayer();

	if (!pPlayer)
		return;

	pPlayer->SetTotalDamage(pPlayer->GetTotalDamage() + pEvent->GetInt("dmg_health"));
	pPlayer->SetTotalHits(pPlayer->GetTotalHits() + 1);
}

GAME_EVENT_F(old_player_hurt)
{
	if (g_bEnableZR)
		ZR_OnPlayerHurt(pEvent);

	if (!g_bEnableTopDefender)
		return;

	CCSPlayerController* pAttacker = (CCSPlayerController*)pEvent->GetPlayerController("attacker");
	CCSPlayerController* pVictim = (CCSPlayerController*)pEvent->GetPlayerController("userid");

	// Ignore Ts/zombies and CTs hurting themselves
	if (!pAttacker || pAttacker->m_iTeamNum() != CS_TEAM_CT || pAttacker->m_iTeamNum() == pVictim->m_iTeamNum())
		return;

	ZEPlayer* pPlayer = pAttacker->GetZEPlayer();

	if (!pPlayer)
		return;

	pPlayer->SetTotalDamage(pPlayer->GetTotalDamage() + pEvent->GetInt("dmg_health"));
	pPlayer->SetTotalHits(pPlayer->GetTotalHits() + 1);
}

GAME_EVENT_F(bomb_planted)
{
	/*
		"bomb_planted":dict({
            "userid":"slot",
            "userid_pawn":"strict_ehandle",
            "site":"short",
            }),
	*/
	CPlayerSlot slot = pEvent->GetPlayerSlot("userid");
	int site = pEvent->GetInt("site");

	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyBombPlanted(pEvent, slot.Get(), site);
}

GAME_EVENT_F(bomb_defused)
{
	/*
	    "bomb_defused":dict({
            "userid":"slot",
            "userid_pawn":"strict_ehandle",
            "site":"short",
            }),
	*/
	CPlayerSlot slot = pEvent->GetPlayerSlot("userid");
	int site = pEvent->GetInt("site");

	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyBombDefused(pEvent, slot.Get(), site);
}

GAME_EVENT_F(bomb_exploded)
{
	/*
		"bomb_exploded":dict({
			"userid":"slot",
			"userid_pawn":"strict_ehandle",
			"site":"short",
			}),
	*/
	CPlayerSlot slot = pEvent->GetPlayerSlot("userid");
	int site = pEvent->GetInt("site");

	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyBombExploded(pEvent, slot.Get(), site);
}

GAME_EVENT_F(player_death)
{
	/*
		"player_death":dict
		({
			"userid":"playercontroller",
			"userid_pawn":"strict_ehandle",
			"attacker":"playercontroller",
			"attacker_pawn":"strict_ehandle",
			"assister":"playercontroller",
			"assister_pawn":"strict_ehandle",
			"assistedflash":"bool",
			"weapon":"string",
			"weapon_itemid":"string",
			"weapon_fauxitemid":"string",
			"weapon_originalowner_xuid":"string",
			"headshot":"bool",
			"dominated":"short",
			"revenge":"short",
			"wipe":"short",
			"penetrated":"short",
			"noreplay":"bool",
			"noscope":"bool",
			"thrusmoke":"bool",
			"attackerblind":"bool",
			"distance":"float",
			"dmg_health":"short",
			"dmg_armor":"byte",
			"hitgroup":"byte",
			"attackerinair":"bool",
		}),
	*/

	//if (g_bEnableZR)
	//	ZR_OnPlayerDeath(pEvent);

	//if (g_bEnableEntWatch)
	//	EW_PlayerDeath(pEvent);

	//if (!g_bEnableTopDefender)
	//	return;

	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerDeath(pEvent);

	CCSPlayerController* pAttacker = (CCSPlayerController*)pEvent->GetPlayerController("attacker");
	CCSPlayerController* pVictim = (CCSPlayerController*)pEvent->GetPlayerController("userid");

	bool noattacker = false;
	if (!pAttacker)
	{
		Message("noattacker=true");
		noattacker = true;
	}

	bool novictim = false;
	if (!pVictim)
	{
		Message("novictim=true");
		novictim = true;
	}
	
	bool teamkill = false;
	if (pAttacker->m_iTeamNum == pVictim->m_iTeamNum)
		teamkill = true;

	bool suicide = false;
	if (pAttacker == pVictim)
		suicide = true;

	// Ignore Ts/zombie kills and ignore CT teamkilling or suicide
	if (	
			noattacker  ||
			novictim    ||
			teamkill    || 
			suicide     ||
			pAttacker->m_iTeamNum != CS_TEAM_CT || 
			pAttacker->m_iTeamNum == pVictim->m_iTeamNum
			)
	{
		return;
	}


	ZEPlayer* pPlayer = pAttacker->GetZEPlayer();

	if (!pPlayer)
		return;

	pPlayer->SetTotalKills(pPlayer->GetTotalKills() + 1);
	
}

GAME_EVENT_F(player_jump)
{
    /*	"player_jump":dict({
			"userid":"playercontroller",
        }),	*/
	int index = pEvent->GetPlayerSlot("userid").Get();
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerJump(index);

	CCSPlayerController* pController = CCSPlayerController::FromSlot(index);

	/* //uncomment if you need to find any of these things again because you forgot!
	//you broke it here likely tristen
	CBaseEntity* pPawn = (CBaseEntity*)pController->GetPawn();
	CCSPlayerPawnBase* ppb = (CCSPlayerPawnBase*)pPawn;
	CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(ppb->m_pItemServices());
	Message("has defuser: %d\n", pItemServices->m_bHasDefuser()); // works
	Message("has helmet: %d\n", pItemServices->m_bHasHelmet()); //works
	Message("has armor: %d\n", pItemServices->m_bHasHeavyArmor()); //works

	CCSPlayerPawn* ccsPB = (CCSPlayerPawn*)pController->GetPawn();
	Message("has m_bIsDefusing: %d\n", ccsPB->m_bIsDefusing()); //not tested
	Message("has m_nWhichBombZone: %d\n", ccsPB->m_nWhichBombZone()); //works  A=1, B=2
	Message("has m_bInBuyZone: %d\n", ccsPB->m_bInBuyZone()); //works, "in buy zone" + "buy time not expired" = true, else false
	Message("has m_bInBombZone: %d\n", ccsPB->m_bInBombZone()); //works, "in bomb zone" + "with bomb" = true, else false.
	*/
}

GAME_EVENT_F(player_land)
{
	int index = pEvent->GetPlayerSlot("userid").Get();
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerLand(index);
}

GAME_EVENT_F(player_airborn)
{
	int index = pEvent->GetPlayerSlot("userid").Get();
	for (auto& plugin : g_CS2Fixes.m_Plugins)
		plugin.PyPlayerAirborn(index);
}

bool g_bFullAllTalk = false;
FAKE_BOOL_CVAR(cs2f_full_alltalk, "Whether to enforce sv_full_alltalk 1", g_bFullAllTalk, false, false);

GAME_EVENT_F(round_start)
{
	g_pPanoramaVoteHandler->Init();

	if (g_bEnableZR)
		ZR_OnRoundStart(pEvent);

	if (g_bEnableLeader)
		Leader_OnRoundStart(pEvent);

	// Dumb workaround for CS2 always overriding sv_full_alltalk on state changes
	if (g_bFullAllTalk)
		g_pEngineServer2->ServerCommand("sv_full_alltalk 1");

	if (!g_bEnableTopDefender || !GetGlobals())
		return;

	for (int i = 0; i < GetGlobals()->maxClients; i++)
	{
		ZEPlayer* pPlayer = g_playerManager->GetPlayer(i);

		if (!pPlayer)
			continue;

		pPlayer->SetTotalDamage(0);
		pPlayer->SetTotalHits(0);
		pPlayer->SetTotalKills(0);
	}
}

GAME_EVENT_F(round_end)
{
	if (g_bVoteManagerEnable)
		g_pVoteManager->OnRoundEnd();

	if (!g_bEnableTopDefender || !GetGlobals())
		return;

	CUtlVector<ZEPlayer*> sortedPlayers;

	for (int i = 0; i < GetGlobals()->maxClients; i++)
	{
		ZEPlayer* pPlayer = g_playerManager->GetPlayer(i);

		if (!pPlayer || pPlayer->GetTotalDamage() == 0)
			continue;

		CCSPlayerController* pController = CCSPlayerController::FromSlot(pPlayer->GetPlayerSlot());

		if (!pController)
			continue;

		sortedPlayers.AddToTail(pPlayer);
	}

	if (sortedPlayers.Count() == 0)
		return;

	sortedPlayers.Sort([](ZEPlayer* const* a, ZEPlayer* const* b) -> int {
		return (*a)->GetTotalDamage() < (*b)->GetTotalDamage();
	});

	ClientPrintAll(HUD_PRINTTALK, " \x09TOP DEFENDERS");

	char colorMap[] = {'\x10', '\x08', '\x09', '\x0B'};

	for (int i = 0; i < sortedPlayers.Count(); i++)
	{
		ZEPlayer* pPlayer = sortedPlayers[i];
		CCSPlayerController* pController = CCSPlayerController::FromSlot(pPlayer->GetPlayerSlot());

		if (i < 5)
			ClientPrintAll(HUD_PRINTTALK, " %c%i. %s \x01- \x07%i DMG \x05(%i HITS & %i KILLS)", colorMap[MIN(i, 3)], i + 1, pController->GetPlayerName(), pPlayer->GetTotalDamage(), pPlayer->GetTotalHits(), pPlayer->GetTotalKills());
		else
			ClientPrint(pController, HUD_PRINTTALK, " \x0C%i. %s \x01- \x07%i DMG \x05(%i HITS & %i KILLS)", i + 1, pController->GetPlayerName(), pPlayer->GetTotalDamage(), pPlayer->GetTotalHits(), pPlayer->GetTotalKills());

		pPlayer->SetTotalDamage(0);
		pPlayer->SetTotalHits(0);
		pPlayer->SetTotalKills(0);
	}
}

GAME_EVENT_F(round_freeze_end)
{
	if (g_bEnableZR)
		ZR_OnRoundFreezeEnd(pEvent);
}

GAME_EVENT_F(round_time_warning)
{
	if (g_bEnableZR)
		ZR_OnRoundTimeWarning(pEvent);
}

GAME_EVENT_F(bullet_impact)
{
	if (g_bEnableLeader)
		Leader_BulletImpact(pEvent);
}

/* The macro below is defined in eventlistener.h
 * #define GAME_EVENT_F(_event)                                        \
 *	void _event##_callback(IGameEvent*);                               \
 *	CGameEventListener _event##_listener(_event##_callback, #_event);  \
 *	void _event##_callback(IGameEvent* pEvent)
 *
 * I sure hope writing this out helps someone else who's in here to figure stuff out.
 *
 * void vote_cast_callback(IGameEvent*);                                   \ definition for the game event
 * CGameEventListener vote_cast_listener(vote_cast_callback, "vote_cast"); \ register that we will have a callback when desired event fires
 * void vote_cast_callback(IGameEvent* pEvent)                             \ the actual callback
 * {
 *     g_pPanoramaVoteHandler->VoteCast(pEvent);
 * }
 * 
 * And that's it, now to forget about this and rediscover in a few years when trying to figure out this magic.
 */
GAME_EVENT_F(vote_cast)
{
	g_pPanoramaVoteHandler->VoteCast(pEvent);
}

GAME_EVENT_F(cs_win_panel_match)
{
	g_pIdleSystem->PauseIdleChecks();

	if (!g_pMapVoteSystem->IsVoteOngoing())
		g_pMapVoteSystem->StartVote();
}

GAME_EVENT_F(player_score)
{
	Message("player_score fired, from where i don't know.\n");
}

GAME_EVENT_F(player_connect)
{
	Message("player_connect\n");
}
GAME_EVENT_F(gc_connected)
{
	Message("gc_connected\n");
}
GAME_EVENT_F(player_connect_full)
{
	Message("player_connect_full\n");
}

GAME_EVENT_F(player_disconnect)
{
	Message("player_disconnect\n");
}
GAME_EVENT_F(client_disconnect)
{
	Message("client_disconnect\n");
}
GAME_EVENT_F(cs_game_disconnected)
{
	Message("cs_game_disconnected\n");
}
