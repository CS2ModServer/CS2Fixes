import Source2Py as s2
GameEvent = s2.GameEvent

from adventure.game_events import game_events
Event = game_events.Event

import logging, inspect
import traceback

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='tests/TestConnections.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging


def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[TestConnections]" + caller + str(message))
    log.info(msg=("[TestConnections]" + caller + str(message)))
    pass

    '''
        "player_connect":dict({
            "name":"string",
            "userid":"playercontroller",
            "networkid":"string",
            "xuid":"uint64",
            "address":"string",
            "bot":"bool",
            }),
        "player_connect_full":dict({
            "userid":"playercontroller",
            }),
        "player_disconnect":dict({
            "userid":"playercontroller",
            "reason":"short",
            "name":"string",
            "networkid":"string",
            "xuid":"uint64",
            "PlayerID":"short",
            }),

    '''
class TestConnections:
    ''' note that python does not need typing in it's method declarations but here it is being used 
        as a reminder of the incoming type from CPP.
    '''
    def OnPluginLoaded(self):
        try:
            alog("=============== TestConnections.py  ==============")
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def player_connect(self, 
        event: GameEvent
        ):
        alog("================= player_connect  ================")
        d = Event(event)
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))
        pass
    def player_connect_full(self, 
        event: GameEvent
        ):
        alog("=============== player_connect_full  =============")
        d = Event(event)
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))
        pass
    def player_disconnect(self,
        event: GameEvent
        ):
        alog("=============== player_disconnect  ===============")
        d = Event(event)
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))
        pass
