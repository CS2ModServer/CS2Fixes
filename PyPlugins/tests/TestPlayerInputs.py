import Source2Py as s2
ADVPlayer   = s2.ADVPlayer

from adventure.game_events import game_events
Event = game_events.Event

import logging, inspect, traceback
logging.basicConfig(filename='tests/TestPlayerInputs.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[TestPlayerInputs]" + caller + str(message))
    log.info(msg=("[TestPlayerInputs]" + caller + str(message)))
    pass


#Put in order of frequency, not sure how python goes through set/dict/list
special_events = {
    "binds": ["ability1", "ability2", "ultimate"],
    "events": ["attacker", "victim", "spawn", "kill", "death"]
}

try:
    from adventure.game_events.trigger_events import TriggerEventManager
    
    event_manager = TriggerEventManager( 
        special_events["binds"], 
        special_events["events"]
        )
    Scroll = event_manager.SM.Scroll
    s      = event_manager.SM.Scroll
except Exception as e:
    alog(e)
    alog(traceback.format_exc())

alog("START")
class TestPlayerInputs(object):
    scroll = None
    def OnPluginLoad(self):
        alog("")
        pass
    @classmethod
    def _give_scroll_ability1(self, slot):
        try:
            # scroll zero is the template scroll, we copy it
            # then set the owner to the spawning player.
            # then tell it to build with that new owner, adding it to their
            # bound pool of technique scrolls
            temp_scroll = s(event_manager.SM.dict_by_id[0])
            temp_scroll.owner = slot

            self.scroll = temp_scroll
            # self.scroll.build(event_manager, category="bind", action_map="use_ability1")
            self.scroll.build(event_manager, action_map="use_ability1")

            alog(" _give_scroll_ability1 ".center(60, "="))
            for k, v in self.scroll.__dict__.items():
                alog(str(k).ljust(25) + " | " + str(v))
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
    @classmethod
    def _give_scroll(self, slot, scrollnum, technum, _action_map): # _category, _action_map):
        temp_scroll = s(event_manager.SM.dict_by_id[scrollnum])
        temp_scroll.owner = slot
        temp_scroll.default_technique_id = technum
        # temp_scroll.build(event_manager, category=_category, action_map=_action_map)
        temp_scroll.build(event_manager, action_map=_action_map)
    def player_spawn(self,
        d
        ):
        slot = d["userid"]
        self._give_scroll_ability1(slot)
        scrollnum = 1 #1 is normal scroll, no modifiers
        technum = 1   #1 is Explode technique
        # self._give_scroll(slot, scrollnum, technum, "passive", "player_death")
        self._give_scroll(slot, scrollnum, technum, "player_death")
        pass
    def player_death(self,
        event):
            d = Event(event)
            success = event_manager.DM.fireBind(d)

    def use_ability1(self,
        d: dict
        ):
        #slot = d["userid"]
        success = event_manager.DM.fireBind(d)
        # if not event_manager.DM.hasBind(slot, d["event_name"]):
        #     event_manager.DM.fireBind(slot, d["event_name"])
        # else:
        #     self._give_scroll_ability1(slot)
    @classmethod
    def _state_info(self,
        d: dict):
        alog(" _state_info ".center(60, "="))
        for k,v in d.items():
            alog(str(k).ljust(25) + " | " + str(v))
        
        p = ADVPlayer(d["userid"])
        if (p.IsValid()):
            bstates = p.GetButtonStates()
            for k,v in s2.InputBitMask_t.__members__.items():
                alog(str(k).ljust(25) + " | " + str(bstates & v))
        pass
    def use_ability2(self,
        d: dict
        ):
        alog(" use_ability2 ".center(60, "="))
        self._state_info(d)
        pass
    def use_ultimate(self,
        d: dict
        ):
        alog(" use_ultimate ".center(60, "="))
        for k, v in d.items():
            alog(str(k).ljust(25) + " | " + str(v))

        userid = d["userid"]
        player = ADVPlayer(userid) 

        if not player.IsValid():
            alog("player.IsValid()".ljust(25) + " | " + "False")
            return False

        pawn = player.pawn
        if not pawn:
            alog("pawn.IsValid()".ljust(25) + " | " + "False")
            return False

        alog("userid: ".ljust(25) + " | " + str(type(userid)))

        distance = 400.0
        alog("distance type: ".ljust(25) + " | " + str(type(distance)))

        ignoreList = [userid,]
        #ignoreList.append(userid)
        alog("ignoreList type: ".ljust(25) + " | " + str(type(ignoreList)))

        pList = list()
        pList = s2.GetPlayersNearPlayerID_list(userid, distance, ignoreList)
        alog("players in {r} range".format(r=distance).ljust(25) + " | " + str(pList))

        distance = distance*2.0
        pDict = s2.GetPlayersNearPlayerID_dict(userid, distance, ignoreList)
        for k, v in pDict.items():
            p2 = ADVPlayer(k)
            if p2.IsValid():
                alog("Distance from {n}.".format(n=ADVPlayer(k).name).ljust(25) + " | " + str(v))
            else:
                alog("ADVPlayer was invalid for".ljust(25) + " | " + str(k))




        
        pass

