import Source2Py
GameEvent = Source2Py.GameEvent
ADVPlayer = Source2Py.ADVPlayer
import logging, inspect, traceback, random

logging.basicConfig(filename='adventure/generic_health.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[generic_health]" + caller + str(message))
    log.info(msg=("[generic_health]" + caller + str(message)))
    pass

def attacker_life_gain_on_hit(
    _event: GameEvent,
    _amount: int,
    ):
    #note the use of dot notation below has specially bound getter and setter to allow access to the CPP function
    #they are still accessible in the normal way too. _event.GetInt("attacker").  Only some of these are available.

    iAttacker = _event.attacker
    if (iAttacker == _event.userid):
        return

    attacker = ADVPlayer(iAttacker)
    attacker.AddHealth(_amount)
    alog("You gained {amount} life on hit!".format(amount=_amount))
    pass

def attacker_life_steal_on_hit(
    _event: GameEvent,
    _chance: float,
    _amount_pct: float
    ):
    iAttacker = _event.attacker
    iVictim   = _event.userid
    if (iAttacker == iVictim):
        return
        
    iDamage = _event.dmg_health
    if (iDamage <= 0):
        return

    _result = random.random()
    if (_result < _chance):
        attacker = ADVPlayer(iAttacker)
        gain = int(iDamage * _amount_pct)
        attacker.AddHealth(gain)
        alog("You stole {num} life from {vic}!".format(num=gain, vic=iVictim))
