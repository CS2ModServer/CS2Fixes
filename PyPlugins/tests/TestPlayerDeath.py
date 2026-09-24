import Source2Py as s2
GameEvent = s2.GameEvent
ADVPlayer = s2.ADVPlayer

import logging, inspect
import traceback

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='tests/TestPlayerDeath.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging


def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[TestPlayerDeath]" + caller + str(message))
    log.info(msg=("[TestPlayerDeath]" + caller + str(message)))
    pass

alog("START")

from tests import dumped_events

#base game events
event_dict_of_dict = dict({})
for k, v in dumped_events.dumped_events.items():
    event_dict_of_dict.update(v)

#This can be iterated over in K/V style, easier than the cs2 IGameEvent*
class Event(object):
    def __setitem__(self, key, value):
        setattr(self, key, value)
    def __getitem__(self, key):
        return getattr(self, key)
    def items(self):
        return self.__dict__.items()
    def __init__(self, 
        _event: GameEvent
        ):
        try:
            self["name"] = _event.GetName()
            keys = event_dict_of_dict.get(self["name"], dict())
            for k, v in keys.items():
                # some or all may be wrong, but working for now
                if   (v == "string"):
                    self[k] = _event.GetString(k, "None")
                elif (v == "bool"):
                    self[k] = _event.GetBool(k, False)
                elif (v == "byte"):
                    self[k] = _event.GetInt(k, -1) 
                elif (v == "short"): 
                    self[k] = _event.GetInt(k, -1) 
                elif (v == "long"): 
                    self[k] = _event.GetInt(k, -1) 
                elif (v == "int"):
                    self[k] = _event.GetInt(k, -1) 
                elif (v == "float"): 
                    self[k] = _event.GetFloat(k, -1.0)
                elif (v == "player_pawn"):
                    self[k] = _event.GetInt(k, -1)
                    #self[k] = _event.GetPlayerPawn(k) "player_footstep":{"userid":"player_pawn"} need to get that event to test if really a pawn or just slot/userid.
                elif (v == "playercontroller"):
                    self[k] = _event.GetPlayerSlot(k).Get()
                elif (v == "uint64"): #player steamid or contentid on steam network
                    self[k] = _event.GetUint64(k, -1)
                elif (v == "strict_ehandle"):
                    self[k] = _event.GetPawnEHandle(k)
                else: 
                    alog("else!!! " + str(k) + " " + str(v))
                    pass
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())

class TestPlayerDeath:
    '''"player_death":dict({
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
                "wipe":"short",       #ace
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
                }),'''
    def OnPluginLoad(self):
        alog("successfully loaded")
        pass
    def _testDeath(self, 
        event: GameEvent
        ):
        ev = Event(event)
        for k, v in ev.items():
            alog(str(k).rjust(25) + "   |   " + str(v).ljust(25))

        #either way is usable because we turned the GameEvent into a python dict like object called Event
        #victim = ADVPlayer(ev['userid'])
        victim = ADVPlayer(event.userid)
        if(victim.IsValid()):
            killstring = victim.name + " was killed by "

            attacker = ADVPlayer(ev['attacker'])
            if (attacker.IsValid()):
                killstring = killstring + attacker.name
                heal = 15
                current_hp = attacker.GetHealth()
                alog(str(attacker.GetName() + " healed by " + str(heal) + " from " + str(current_hp) + " to " + str(current_hp+heal)))
                attacker.AddHealth(15)

            assister = ADVPlayer(ev['assister'])
            if (assister.IsValid()):
                name = assister.GetName()
                if (name):
                    killstring = killstring + ", and " + assister.GetName()
                heal = 5
                current_hp = assister.GetHealth()
                alog(str(assister.GetName() + " healed by " + str(heal) + " from " + str(current_hp) + " to " + str(current_hp+heal)))
                assister.AddHealth(5)

            alog(killstring)
        pass
    def player_death(self, 
        event: GameEvent
        ):
        alog("START")
        try:
            self._testDeath(self, event)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
