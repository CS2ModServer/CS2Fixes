/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2026 Source2ZE
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
#include "hud_manager.h"
#include "idlemanager.h"
#include "leader.h"
#include "map_votes.h"
#include "mapmigrations.h"
#include "panoramavote.h"
#include "recipientfilters.h"
#include "topdefender.h"
#include "votemanager.h"
#include "zombiereborn.h"
#include "adventuremod.h"

#include "cs2fixes.h"

#include "tier0/memdbgon.h"

std::vector<CGameEventListener*> g_vecEventListeners;

void RegisterEventListeners()
{
	static bool bRegistered = false;

	if (bRegistered || !g_gameEventManager)
		return;
	
	//E:/CS2Server/cs2/game/csgo/bin/win64/~ is where were at normally.
	g_gameEventManager->LoadEventsFromFile("../../csgo/addons/CS2Fixes/resource/adventure.res", false);

	for (CGameEventListener* pListener : g_vecEventListeners)
		g_gameEventManager->AddListener(pListener, pListener->GetEventName(), true);

	bRegistered = true;
}

void UnregisterEventListeners()
{
	if (!g_gameEventManager)
		return;

	for (CGameEventListener* pListener : g_vecEventListeners)
		g_gameEventManager->RemoveListener(pListener);

	g_vecEventListeners.clear();
}

GAME_EVENT_F(round_prestart)
{
	RemoveTimers(TIMERFLAG_ROUND);

	EntityHandler_OnRoundRestart();

	CBaseEntity* pShake = nullptr;

	// Prevent shakes carrying over from previous rounds
	while ((pShake = UTIL_FindEntityByClassname(pShake, "env_shake")))
		pShake->AcceptInput("StopShake");

	if (g_cvarEnableZR.Get())
		ZR_OnRoundPrestart(pEvent);

	if (g_cvarEnableEntWatch.Get())
		EW_RoundPreStart();

	g_pMapMigrations->OnRoundPrestart();
}

CConVar<bool> g_cvarBlockTeamMessages("cs2f_block_team_messages", FCVAR_NONE, "Whether to block team join messages", false);

GAME_EVENT_F(player_team)
{
	// Remove chat message for team changes
	if (g_cvarBlockTeamMessages.Get())
		pEvent->SetBool("silent", true);
}

CConVar<bool> g_cvarNoblock("cs2f_noblock_enable", FCVAR_NONE, "Whether to use player noblock, which sets debris collision on every player", false);
CConVar<int> g_cvarFreeArmor("cs2f_free_armor", FCVAR_NONE, "Whether kevlar (1+) and/or helmet (2) are given automatically", 0, true, 0, true, 2);

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

	if (g_cvarEnableZR.Get())
		ZR_OnPlayerSpawn(pController);

	if (pController->IsConnected())
		pController->GetZEPlayer()->OnSpawn();

	CHandle<CCSPlayerController> hController = pController->GetHandle();

	// Gotta do this on the next frame...
	CTimer::Create(0.0f, TIMERFLAG_MAP | TIMERFLAG_ROUND, [hController]() {
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
		if (!g_cvarNoblock.Get() || !pPawn || !pPawn->IsAlive())
			return -1.0f;

		pPawn->SetCollisionGroup(COLLISION_GROUP_DEBRIS);

		return -1.0f;
	});

	CCSPlayerPawn* pPawn = (CCSPlayerPawn*)pController->GetPawn();

	if (!pPawn)
		return;

	CCSPlayer_ItemServices* pItemServices = pPawn->m_pItemServices();

	if (!pItemServices)
		return;

	// Dumb workaround for mp_free_armor breaking kevlar rebuys in buy menu
	if (g_cvarFreeArmor.GetInt() == 1)
		pItemServices->GiveNamedItem("item_kevlar");
	else if (g_cvarFreeArmor.GetInt() == 2)
		pItemServices->GiveNamedItem("item_assaultsuit");

}

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

	//if (!g_bEnableTopDefender)
	//		return;

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
	if (g_cvarEnableTopDefender.Get())
		TD_OnPlayerHurt(pEvent);
}

GAME_EVENT_F(old_player_hurt)
{
	if (g_cvarEnableTopDefender.Get())
		TD_OnPlayerHurt(pEvent);

	if (!g_cvarEnableTopDefender.Get())
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
	if (g_cvarEnableEntWatch.Get())
		EW_PlayerDeath(pEvent);

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
	if (g_cvarEnableTopDefender.Get())
		TD_OnPlayerDeath(pEvent);
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

CConVar<bool> g_cvarFullAllTalk("cs2f_full_alltalk", 0, "Whether to enforce sv_full_alltalk 1", false);

GAME_EVENT_F(round_start)
{
	g_pPanoramaVoteHandler->Init();

	if (g_cvarEnableZR.Get())
		ZR_OnRoundStart(pEvent);

	if (g_cvarEnableLeader.Get())
		Leader_OnRoundStart(pEvent);

	// Dumb workaround for CS2 always overriding sv_full_alltalk on state changes
	if (g_cvarFullAllTalk.Get())
		g_pEngineServer2->ServerCommand("sv_full_alltalk 1");

	// Ensure there's no warmup, because mp_warmup_online_enabled gets randomly ignored for some reason, this is a problem with cs2f_fix_hud_flashing
	if (g_cvarFixHudFlashing.Get() && g_pGameRules && g_pGameRules->m_bWarmupPeriod)
		g_pEngineServer2->ServerCommand("mp_warmup_end");

	if (g_cvarEnableTopDefender.Get())
		TD_OnRoundStart(pEvent);
}

GAME_EVENT_F(round_end)
{
	if (g_cvarFixHudFlashing.Get() && g_pGameRules)
		g_pGameRules->m_bGameRestart = false;

	if (g_cvarEnableTopDefender.Get())
		TD_OnRoundEnd(pEvent);
}

GAME_EVENT_F(round_freeze_end)
{
	if (g_cvarEnableZR.Get())
		ZR_OnRoundFreezeEnd(pEvent);
}

GAME_EVENT_F(round_time_warning)
{
	if (g_cvarEnableZR.Get())
		ZR_OnRoundTimeWarning(pEvent);
}

GAME_EVENT_F(bullet_impact)
{
	if (g_cvarEnableLeader.Get())
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
