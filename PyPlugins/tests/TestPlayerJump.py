import Source2Py as s2
ADVPlayer   = s2.ADVPlayer
GameEvent   = s2.GameEvent
CBaseEntity = s2.CBaseEntity

from adventure.game_events import game_events
Event = game_events.Event

import logging, inspect, traceback, random

logging.basicConfig(filename='tests/TestPlayerJump.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[TestPlayerJump]" + caller + str(message))
    log.info(msg=("[TestPlayerJump]" + caller + str(message)))
    pass

alog("START")
class TestPlayerJump:
    bonus = {-1: 1.0} #int: float
    jump_type  = {-1: -1} #int: int
    jump_name  = {-1: "unassigned"} #int: str
    def OnPluginLoad(self):
        alog("loaded!")
        pass
    @classmethod
    def _assign_bonus(self, slot):
        #around 300.0 is the top end of z velocity when jumping.
        
        self.jump_type[slot] = random.randint(1,2)
        if (self.jump_type[slot] is 1):
            self.jump_name[slot] = "longjump"
            self.bonus[slot] = float(random.randint(150, 250) / 100)
            pass
        elif (self.jump_type[slot] is 2):
            self.jump_name[slot] = "highjump"
            self.bonus[slot] = float(random.randint(125, 225))
            pass

        alog(str("jump_type").ljust(12) + " | " + str(self.jump_type[slot]))
        alog(str("jump_name").ljust(12) + " | " + str(self.jump_name[slot]))
        alog(str("bonus").ljust(12) + " | " +     str(self.bonus[slot]))
        pass
    @classmethod
    def _jump_high(self, slot):
        p = ADVPlayer(slot)
        pawn = p.GetPawn()
        if not pawn:
            return
        
        velocitydict = pawn.absvelocity
        pushdict = {
            "x": velocitydict["x"], 
            "y": velocitydict["y"], 
            "z": self.bonus[slot] + velocitydict["z"]
            }
        pawn.Push(pushdict)
        pass
    @classmethod
    def _jump_long(self, slot):
        p = ADVPlayer(slot)
        pawn = p.GetPawn()
        if not pawn:
            return
        
        velocitydict = pawn.localvelocity
        pushdict = {
            "x": velocitydict["x"]*self.bonus[slot], 
            "y": velocitydict["y"]*self.bonus[slot], 
            "z": velocitydict["z"]
            }
        pawn.Push(pushdict)
        pass
    @classmethod
    def _state_info(self,
        d: dict):
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))

        slot = d["userid"]
        p = ADVPlayer(slot)
        alog(str("IsOnGround").ljust(14) + " | " + str(p.IsOnGround()))
        alog(str("IsOnLadder").ljust(14) + " | " + str(p.IsOnLadder()))
        pawn = p.pawn
        if not pawn:
            return
        alog(str("pawn").ljust(14) + " | " + str(pawn))
        alog(str("absvelocity").ljust(14) + " | " + str(pawn.absvelocity))
        alog(str("jump_type").ljust(14) + " | " + str(self.jump_type[slot]))
        alog(str("jump_name").ljust(14) + " | " + str(self.jump_name[slot]))
        alog(str("bonus").ljust(14) + " | " +     str(self.bonus[slot]))
        pass
    def player_jump(self, 
        event
        ):
        ''' "player_jump":dict({
            "userid":"playercontroller",
            }), '''

        slot = event.userid
        alog(" player_jump ".center(60, "="))

        try:
            if slot not in self.jump_type.keys():
                self._assign_bonus(slot)
        except KeyError as e:
            alog(e)
            alog(traceback.format_exc())        
            self._assign_bonus(slot)


        if (self.jump_type[slot] is 1):
            self._jump_long(slot)
        elif (self.jump_type[slot] is 2):
            self._jump_high(slot)

        d = Event(event)
        self._state_info(d)
        pass
    def player_airborn(self,
        event
        ):
        ''' "player_airborn":dict({
            "userid":"playercontroller",
            }), '''
        alog(" player_airborn ".center(60, "="))
        d = Event(event)
        self._state_info(d)
        pass
    def player_land(self,
        event
        ):
        ''' "player_land":dict({
            "userid":"playercontroller",
            }), '''
        alog(" player_land ".center(60, "="))
        d = Event(event)
        self._state_info(d)
        pass
    def player_spawn_fake(self,
        d: dict
        ):
        slot = d["userid"]
        player = ADVPlayer(slot)

        if not player.IsValid():
            return 

        if player.team not in [2, 3]:
            return

        if not player.IsAlive():
            return

        # player has (re)spawned into a round and is alive
        pawn = player.GetPawn()
        if not pawn:
            return

        alog(" player_spawn_fake ".center(60, "="))
        self._assign_bonus(slot)
        self._state_info(d)
        pass
