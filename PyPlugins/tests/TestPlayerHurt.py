import Source2Py
GameEvent = Source2Py.GameEvent
from adventure.generic_damage import generic_damage
from adventure.generic_health import generic_health

import logging, inspect
import traceback

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='tests/TestPlayerHurt.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging


def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestPlayerHurt]" + caller + str(message))
    log.info(msg=("[TestPlayerHurt]" + caller + str(message)))
    pass

class TestPlayerHurt:
    '''"player_hurt":dict({
            "userid":           GetInt //they say it's "playercontroller" but it isn't, its the playerindex
            "userid_pawn":      Get??? // "strict_ehandle",
            "attacker":         GetInt //they say it's "playercontroller" but it isn't, its the playerindex
            "attacker_pawn":    Get??? // "strict_ehandle",
            "health":           GetInt // "byte",
            "armor":            GetInt // "byte",
            "weapon":           GetStr // name of the weapon used "string",
            "dmg_health":       GetInt // health damage dealt with the player_hurt "short",
            "dmg_armor":        GetInt // armor damage dealt with the player_hurt "byte",
            "hitgroup":         GetInt // 1=head, 2=body_upper, 3=body_lower, 4=arm_left, 5=arm_right, 6=leg_left, 7=leg_right
            }),
    '''
    ''' note that python does not need typing in it's method declarations but here it is being used 
        as a reminder of the incoming type from CPP.
    '''
    def OnPluginLoad(self):
        alog("successfully loaded")
        alog("pre_, post_, and normal player_hurt")
        alog("After the CPP event starts, the python event")
        alog("is called and completed, then the cpp event ")
        alog("concludes.  Even though the CPP event fires")
        alog("before PY the damage/ and IGameEvent pEvent")
        alog("can be modified in Python.")
        pass 
    def player_hurt(self, 
        event: GameEvent
        ):
        try:
            # 50% chance of +4 damage on hit.
            generic_damage.damage_bonus_flat(event, 0.50, 4)
            # 20% chance of +150% or multiplying the damage by 250% damage on hit
            generic_damage.damage_bonus_multiplier(event, 0.20, 2.5)
            # heal 5 hp on hit if it wasn't self.
            generic_health.attacker_life_gain_on_hit(event, 5)
            # 15% chance to gain 40% of damage dealt as life
            generic_health.attacker_life_steal_on_hit(event, 0.15, 0.40)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
