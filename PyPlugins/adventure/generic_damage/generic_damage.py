import Source2Py as s2
GameEvent = s2.GameEvent
ADVPlayer = s2.ADVPlayer
from adventure.generic_stats import generic_stats
import logging, inspect, traceback, random

logging.basicConfig(filename='adventure/generic_damage.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[generic_damage]" + caller + str(message))
    log.info(msg=("[generic_damage]" + caller + str(message)))
    pass

def damage_bonus_flat(
    event: GameEvent,
    chance: float=1.0,
    amount: int=1
    ):
    try:
        iAttacker = event.attacker
        iVictim   = event.userid
        if (iAttacker == iVictim ):
            return False

        attacker = ADVPlayer(iAttacker)
        victim   = ADVPlayer(iVictim)
        if (attacker.team == victim.team):
            return False

        result = random.random()
        if (result < chance):
            s2.PrintToChat(iAttacker, "You dealt {added} extra damage to {vic}!"
                .format(added=amount, vic=victim.name))
            s2.PrintToChat(iVictim, "{att} dealt {added} extra damage to you!"
                .format(added=amount, att=attacker.name))
            generic_stats.changeLife(event, victim.slot, 0-amount)
            return True
        return False
    except Exception as e:
        alog(e)
        alog(traceback.format_exc())
        return False
def damage_bonus_multiplier(
    event: GameEvent,
    chance: float=0.10,
    multiplier: float=1.20
    ):
    try:
        iAttacker = event.attacker
        iVictim   = event.userid
        if (iAttacker == iVictim ):
            return False

        attacker = ADVPlayer(iAttacker)
        victim   = ADVPlayer(iVictim)
        if (attacker.team == victim.team):
            return False

        result = random.random()
        if (result < chance):
            bonus = int(event.dmg_health * multiplier)
            s2.PrintToChat(iAttacker, "You crit {vic} for {b} bonus damage!"
                .format(vic=victim.name,b=bonus))
            s2.PrintToChat(iVictim, "{att} landed a crit on you for {b} bonus damage!"
                .format(att=attacker.name,b=bonus))
            generic_stats.changeLife(event, victim.slot, 0-bonus)
            return True
        return False
    except Exception as e:
        alog(e)
        alog(traceback.format_exc())
        return False
