import Source2Py as s2
GameEvent = s2.GameEvent
ADVPlayer = s2.ADVPlayer
from adventure.generic_stats import generic_stats
import logging, inspect, traceback, random

logging.basicConfig(filename='adventure/generic_health.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[generic_health]" + caller + str(message))
    log.info(msg=("[generic_health]" + caller + str(message)))
    pass

def attacker_life_gain_on_hit(
    _event: GameEvent,
    _chance: float,
    _amount_flat: int,
    ):
    #note the use of dot notation below has specially bound getter and setter to allow access to the CPP function
    #they are still accessible in the normal way too. _event.GetInt("attacker").  Only some of these are available
    #but they may be seen in PyModule.h
    try:
        iAttacker = _event.attacker
        iVictim   = _event.userid
        if (iAttacker == iVictim ):
            return False

        attacker = ADVPlayer(iAttacker)
        victim   = ADVPlayer(iVictim)
        if (attacker.team == victim.team):
            return False

        _result = random.random()
        if (_result < _chance):
            generic_stats.changeLife(_event, attacker.slot, _amount_flat)
            s2.PrintToChat(iAttacker, "You gained {amount} life on hit!"
                .format(amount=_amount_flat))
            return True
        return False
    except Exception as e:
        alog(e)
        alog(traceback.format_exc())
        return False
def attacker_life_steal_on_hit(
    _event: GameEvent,
    _chance: float,
    _amount_percent: float
    ):
    try:
        iAttacker = _event.attacker
        iVictim   = _event.userid
        if (iAttacker == iVictim ):
            return False

        attacker = ADVPlayer(iAttacker)
        victim   = ADVPlayer(iVictim)
        if (attacker.team == victim.team):
            return False

        iDamage = _event.dmg_health
        if (iDamage <= 0):
            return False

        _result = random.random()
        if (_result < _chance):
            gain = int(iDamage * _amount_percent)

            generic_stats.changeLife(_event, attacker.slot, gain)
            s2.PrintToChat(iAttacker, "You stole {num} life from {vic}!"
                .format(num=gain, vic=victim.name))
            s2.PrintToChat(iVictim, "{att} stole {num} life from you!"
                .format(num=gain,att=attacker.name))
            return True
    except Exception as e:
        alog(e)
        alog(traceback.format_exc())
        return False
