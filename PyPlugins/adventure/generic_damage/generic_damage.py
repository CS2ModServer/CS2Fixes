import Source2Py
GameEvent = Source2Py.GameEvent

import logging, inspect, traceback, random

logging.basicConfig(filename='adventure/generic_damage.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[generic_damage]" + caller + str(message))
    log.info(msg=("[generic_damage]" + caller + str(message)))
    pass

def damage_bonus_flat(
    _event: GameEvent,
    _chance: float=1.0,
    _amount: int=1
    ):
    _result = random.random()
    if (_result < _chance):
        attacker = _event.attacker
        victim = _event.userid
        _base =  _event.dmg_health
        alog("attacker={att} dealt victim={vic} {added} extra damage".format(att=attacker, vic=victim, added=_amount))
        _event.dmg_health = int(_base + _amount)
    pass

def damage_bonus_multiplier(
    _event: GameEvent,
    _chance: float=0.10,
    _multiplier: float=1.20
    ):
    _result = random.random()
    if (_result < _chance):
        attacker = _event.attacker
        victim = _event.userid
        _base =  _event.dmg_health
        _new_damage = int(_base * _multiplier)
        alog("{att} landed a crit on {vic} for {bonus} bonus damage".format(att=attacker, vic=victim, bonus=(_new_damage - _base)))
        _event.dmg_health = int(_new_damage)
    pass
