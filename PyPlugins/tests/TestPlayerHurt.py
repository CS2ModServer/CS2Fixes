import Source2Py as s2
GameEvent = s2.GameEvent
ADVPlayer = s2.ADVPlayer
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
    s2.ServerPrint("[TestPlayerHurt]" + caller + str(message))
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
        pass 
    def player_hurt(self, 
        event: GameEvent
        ):
        try:
            attacker = ADVPlayer(event.attacker)
            victim = ADVPlayer(event.victim)
            if (   attacker.team < 2
                or attacker.team > 3
                or   victim.team > 2
                or   victim.team > 3
                ):
                return

            # 20% chance of +4 damage on hit
            if (generic_damage.damage_bonus_flat(event, 0.20, 4)):
                return

            # 20% chance of +150% or multiplying the damage by 150% damage on hit
            if (generic_damage.damage_bonus_multiplier(event, 0.20, 1.5)):
                return

            # 15% chance to heal 15 hp on hit
            if (generic_health.attacker_life_gain_on_hit(event, 0.15, 15)):
                return

            # 15% chance to gain 40% of damage dealt as life
            if (generic_health.attacker_life_steal_on_hit(event, 0.15, 0.40)):
                return

            pass
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def player_take_damage(self, 
        d: dict #not a game event but has a py::dict with info passed from a detour
        ):
        try:
            if (d[  'victim_class'] != 'player' or
                d['attacker_class'] != 'player'):
                return
            iVic = d['victim_id']
            victim = ADVPlayer(iVic)
            if (victim.IsValid() == False):
                #alog("victim not valid")
                return

            life = victim.life
            damage = int(d['damage'])
            newlife = life-damage
            #alog("life{l}-{d}={newl} {v}"
            #    .format(v=victim.name, l=life, d=damage, newl=newlife))
            #alog(d.items())
            pass
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
