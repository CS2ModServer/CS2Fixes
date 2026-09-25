import Source2Py as s2
ADVPlayer   = s2.ADVPlayer
GameEvent   = s2.GameEvent

from adventure.game_events import game_events
Event = game_events.Event

import logging, inspect

from random import randint

logging.basicConfig(filename='tests/TestPlayerSpawn.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[TestPlayerSpawn]" + caller + str(message))
    log.info(msg=("[TestPlayerSpawn]" + caller + str(message)))
    pass

alog("START")
class TestPlayerSpawn:
    def __init__(self):
        pass
    ''' note that python does not need typing in it's method declarations but here it is being used 
        as a reminder of the incoming type from CPP.
    '''
    def OnPluginLoad(self):
        alog(" TestPlayerSpawn.py ".center(60, "="))
        pass 
    def _state_info(slot):
        p = ADVPlayer(slot)
        alog(str("IsAlive").ljust(14) + " | " + str(p.IsAlive()))
        alog(str("IsValid").ljust(14) + " | " + str(p.IsValid()))
        alog(str("IsBot").ljust(14) + " | " + str(p.IsBot()))

        #CS_TEAM_NONE      0
        #CS_TEAM_SPECTATOR 1
        #CS_TEAM_T         2
        #CS_TEAM_CT        3
        alog(str("Team").ljust(14) + " | " + str(p.team))
        alog(str("Life").ljust(14) + " | " + str(p.life))

        pass
    def player_activate(self, 
        event: GameEvent
        ):
        # Finished loading into server, players game client also fully loaded

        alog(" player_activate ".center(60, "="))
        d = Event(event)
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))
        self._state_info(event.userid)
        pass
    def player_spawned(self, 
        event: GameEvent
        ):
        # server done loading player entity
        alog(" player_spawned ".center(60, "="))
        d = Event(event)
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))
        self._state_info(event.userid)
        pass
    def player_spawn_fake(self,
        d: dict
        ):
        slot = d["userid"]
        player = ADVPlayer(slot)

        if not player.IsValid():
            return 

        if player.team not in [2, 3]:
            return

        if not player.IsAlive():
            return

        # player has (re)spawned into a round and is alive
        if not player.life == 100:
            return

        # player hasn't had anything alter their life so they don't yet have a bonus.
        bonus_hp = randint(10, 50)
        player.AddHealth(bonus_hp)

        alog(" player_spawn_fake ".center(60, "="))
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))

        self._state_info(slot)
        pass
