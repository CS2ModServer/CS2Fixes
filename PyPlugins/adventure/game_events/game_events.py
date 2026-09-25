import Source2Py as s2
GameEvent = s2.GameEvent
import traceback

from tests import dumped_events

import logging, inspect
logging.basicConfig(filename='adventure/game_events/game_events.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[game_events]" + caller + str(message))
    log.info(msg=("[game_events]" + caller + str(message)))
    pass

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
            self["event_name"] = _event.GetName()
            keys = event_dict_of_dict.get(self["event_name"], dict())
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
