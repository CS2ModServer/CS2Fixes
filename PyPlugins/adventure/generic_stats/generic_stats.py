import Source2Py as s2
GameEvent = s2.GameEvent
ADVPlayer = s2.ADVPlayer

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

def changeLife(
    _event: GameEvent,
    slot,
    change
    ):
    try:
        p=ADVPlayer(slot)
        current = p.life
        p.life = current+change
        if (p.life <= 0):
            #alog("{p} died with {l} life remaining."
            #    .format(p=p.name,l=p.life))
            kill(p.slot)
        pass
    except Exception as e:
        alog(e)
        alog(traceback.format_exc())
        pass
def kill(
    slot
    ):
    try:
        #alog("{p} died.".format(p=ADVPlayer(slot).name))
        s2.ClientCommand(slot, "kill")
        pass
    except Exception as e:
        alog(e)
        alog(traceback.format_exc())
        pass
