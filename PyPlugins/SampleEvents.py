import Source2Py
import logging, inspect
import traceback

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging


def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[SampleEvents]" + caller + str(message))
    log.info(msg=("[SampleEvents]" + caller + str(message)))
    pass


#TRISTEN YOU LEFT OFF HERE
try:
    from ./tests import dumped_events
except Exception as e:
    alog(e)


#base game events
event_dict_of_dict = dict({})
for k, v in dumped_events.dumped_events.items():
    event_dict_of_dict.update(v)
#fake events
'''        
    "player_hurt":dict({
        "userid":"playercontroller",
        "userid_pawn":"strict_ehandle",
        "attacker":"playercontroller",
        "attacker_pawn":"strict_ehandle",
        "health":"byte",
        "armor":"byte",
        "weapon":"string",
        "dmg_health":"short",
        "dmg_armor":"byte",
        "hitgroup":"byte",
        }),
'''
event_dict_of_dict.update({"player_hurt_pre":event_dict_of_dict["player_hurt"]})
event_dict_of_dict.update({"player_hurt_post":event_dict_of_dict["player_hurt"]})

class Event(object):
    #ev = dict()
    def __setitem__(self, key, value):
        setattr(self, key, value)
    def __getitem__(self, key):
        return getattr(self, key)
    def items(self):
        return self.__dict__.items()
    def __init__(self, _event):
        #alog("START")
        #self.__dict__.clear()
        try:
            self["name"] = _event.GetName()
            keys = event_dict_of_dict.get(self["name"], dict())
            for k, v in keys.items():
                GEKS = Source2Py.GameEventKeySymbol_t(k)

                # some or all may be wrong, but working for now
                if   (v == "string"):
                    self[k] = _event.GetString(GEKS, "None")

                elif (v == "bool"):
                    self[k] = _event.GetBool(GEKS, False)

                elif (v == "byte"):
                    self[k] = _event.GetInt(GEKS, -1)      #python cares not, will automagically sort this mess
                elif (v == "short"):                       #mess
                    self[k] = _event.GetInt(GEKS, -1)      #mess
                elif (v == "long"):                        #mess
                    self[k] = _event.GetInt(GEKS, -1)      #mess
                elif (v == "int"):                         #mess
                    self[k] = _event.GetInt(GEKS, -1)      #mess
                elif (v == "float"):                       #mess
                    self[k] = _event.GetFloat(GEKS, -1.0)  #mess
                elif (v == "player_pawn"):                 #mess
                    self[k] = _event.GetInt(GEKS, -1)      #mess
                elif (v == "playercontroller"):                     #universally used as player id on server for all events (so far)
                    self[k] = _event.GetInt(GEKS, -1)

                elif (v == "uint64"):                               #player steamid or contentid on steam network
                    self[k] = _event.GetUint64(GEKS, 0)

                elif (v == "strict_ehandle"):
                    #GetEntity is used for _pawn keys like attacker_pawn and userid_pawn which are the only ones currently using strict_ehandle
                    #temp = Source2Py.CEntityInstance
                    self[k] = _event.GetEntity(GEKS, None)

                    # works correctly for now?
                    #temp = Source2Py.CEntityHandle()
                    #self[k] = _event.GetEHandle( GEKS,  temp)
                    pass
                else: #   local, hint_, 
                    alog("else!!! " + str(k) + " " + str(v))
                    pass
                #alog("K: " + k + " V: " + v + ": " + str(self[k]))
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        #alog("END\n\n")

events_whitelist = list([
    "player_spawn",
    "player_shoot",
    "player_hurt_pre",
    "player_hurt",
    "player_hurt_post",
    "player_death",
    ])
empty_events_list = list([])
class SampleEvents:
    def __init__(self):
        #this does not get called by Source2Py as far as I am aware.
        pass
    def player_hurt(self, event): 
        '''
            "player_hurt":dict({
                "userid":"playercontroller",
                "userid_pawn":"strict_ehandle",
                "attacker":"playercontroller",
                "attacker_pawn":"strict_ehandle",
                "health":"byte",
                "armor":"byte",
                "weapon":"string",
                "dmg_health":"short",
                "dmg_armor":"byte",
                "hitgroup":"byte",
                }),
        '''
        ev = Event(event)
        alog("dmg_health was: " + str(ev["dmg_health"]))
        ev["dmg_health"] = 99
        aPawn = ev["userid_pawn"]
        alog("dmg_health is now: " + str(ev["dmg_health"]))
        pass
    def player_hurt_pre(self, event):
        alog("-")
        try:
            alog("try")
            ev = Event(event)
            alog("dmg_health was: " + str(ev["dmg_health"]))
            ev["dmg_health"] = 99
            aPawn = ev["userid_pawn"]
            alog("dmg_health is now: " + str(ev["dmg_health"]))
        except Exception as e:
            alog("exception")
            alog(e.message)
            alog(e.args)
        pass
    def player_hurt_post(self, event):
        alog("-")
        ev = Event(event)
        alog("dmg_health was: " + str(ev["dmg_health"]))
        ev["dmg_health"] = 99
        aPawn = ev["userid_pawn"]
        alog("dmg_health is now: " + str(ev["dmg_health"]))
        pass
    def OnPluginLoad(self, printmymethods: bool = False):
        #alog("TEST" + str(dumped_events.dumped_events) + "TEST")

        #alog("Logging active, use alog(\"message\") to log a message or information.")



        alog("-")

        if (printmymethods):
            alog(str(self.__name__))
            for k, v in self.__dict__.items():
                alog("k: " + str(k) + " v: " + str(v))

        pass
    def OnPluginUnload(self):
        alog("Empty events fired during play: ")
        for name in empty_events_list:
            alog(str(name))
        pass
    def _player_spawn(self, ev):
        pass
    def _player_shoot(self, ev):
        pass
    def _player_death(self, ev):
        pass
    def OnGameEvent(self, event):
        #alog("pre")
        ev = Event(event)
        alog(ev["name"])
        if (len(ev.__dict__) <= 1):
            empty_events_list.append(ev["name"])
        elif (ev["name"] in events_whitelist):
            temp_method = getattr(self, "_"+ev["name"])
            temp_method(self, ev)
        #else:
        #    alog("Ignored event: '" + ev["name"] + "'")

        if (ev["name"] == "player_hurt"):
            GEKS = Source2Py.GameEventKeySymbol_t("dmg_health")
            alog("dmg_health is now: " + str(ev   ["dmg_health"]))
            event.SetInt(GEKS, 99)
            
            alog("dmg_health finally is: " + str(event.GetInt(GEKS, -1)))





