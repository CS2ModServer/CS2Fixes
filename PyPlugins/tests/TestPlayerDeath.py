import Source2Py
import logging, inspect
import traceback

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='tests/TestPlayerDeath.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging


def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestPlayerDeath]" + caller + str(message))
    log.info(msg=("[TestPlayerDeath]" + caller + str(message)))
    pass

alog("START")

from tests import dumped_events

#base game events
event_dict_of_dict = dict({})
for k, v in dumped_events.dumped_events.items():
    event_dict_of_dict.update(v)

class Event(object):
    def __setitem__(self, key, value):
        setattr(self, key, value)
    def __getitem__(self, key):
        return getattr(self, key)
    def items(self):
        return self.__dict__.items()
    def __init__(self, _event):
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
                elif (v == "playercontroller"):            #universally used as player id on server for all events (so far)
                    self[k] = _event.GetPlayerSlot(GEKS).Get()
                    #self[k] = _event.GetInt(GEKS, -1)

                elif (v == "uint64"):                      #player steamid or contentid on steam network
                    self[k] = _event.GetUint64(GEKS, 0)

                elif (v == "strict_ehandle"):
                    #GetEntity is used for _pawn keys like attacker_pawn and userid_pawn which are the only ones currently using strict_ehandle
                    #self[k] = _event.GetEntity(GEKS, None)
                    pass
                else: #   local, hint_, 
                    alog("else!!! " + str(k) + " " + str(v))
                    pass
                #alog("K: " + k + " V: " + v + ": " + str(self[k]))        except Exception as e:
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())

class TestPlayerDeath:
    '''"player_death":dict({
                "userid":"playercontroller",
                "userid_pawn":"strict_ehandle",
                "attacker":"playercontroller",
                "attacker_pawn":"strict_ehandle",
                "assister":"playercontroller",
                "assister_pawn":"strict_ehandle",
                "assistedflash":"bool",
                "weapon":"string",
                "weapon_itemid":"string",
                "weapon_fauxitemid":"string",
                "weapon_originalowner_xuid":"string",
                "headshot":"bool",
                "dominated":"short",
                "revenge":"short",
                "wipe":"short",
                "penetrated":"short",
                "noreplay":"bool",
                "noscope":"bool",
                "thrusmoke":"bool",
                "attackerblind":"bool",
                "distance":"float",
                "dmg_health":"short",
                "dmg_armor":"byte",
                "hitgroup":"byte",
                "attackerinair":"bool",
                }),'''
    def OnPluginLoad(self):
        alog("successfully loaded")
        pass
    def _heal(self, who, amount):
        player = Source2Py.ADVPlayer(who)
        player.AddHealth(amount)
    def _testDeath_old(self, event):
        geks = Source2Py.GameEventKeySymbol_t("userid")
        playerid = event.GetPlayerSlot(geks).Get()
        who_died = Source2Py.ADVPlayer(playerid)

        geks = Source2Py.GameEventKeySymbol_t("attacker")
        attackerid = event.GetPlayerSlot(geks).Get()
        self._heal(self, attackerid, 35)

        geks = Source2Py.GameEventKeySymbol_t("assister")
        assisterid = event.GetPlayerSlot(geks).Get()
        if (assisterid>=0):
            self._heal(self, attackerid, 5)

        alog("player slot:" + str(int(playerid)) + " name:" + str(who_died.GetName()))
        pass
    def _testDeath_new(self, event):
        ev = Event(event)
        for k, v in ev.items():
            alog(str(k) + "   |   " + str(v))

        victim = Source2Py.ADVPlayer(ev['userid'])
        
        if(victim.IsValid()):
            killstring = victim.GetName() + " was killed by "

            attacker = Source2Py.ADVPlayer(ev['attacker'])
            if (attacker.IsValid()):
                killstring = killstring + attacker.GetName()
                heal = 15
                current_hp = attacker.GetHealth()
                alog(str(attacker.GetName() + " healed by " + str(heal) + " from " + str(current_hp) + " to " + str(current_hp+heal)))
                attacker.AddHealth(15)

            assister = Source2Py.ADVPlayer(ev['assister'])
            if (assister.IsValid()):
                name = assister.GetName()
                if (name):
                    killstring = killstring + ", and " + assister.GetName()
                heal = 5
                current_hp = assister.GetHealth()
                alog(str(assister.GetName() + " healed by " + str(heal) + " from " + str(current_hp) + " to " + str(current_hp+heal)))
                assister.AddHealth(5)

            alog(killstring)
        pass
    def OnPlayerDeath(self, event):
        alog("START")
        try:
            #self._testDeath_old(self, event)
            self._testDeath_new(self, event)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass