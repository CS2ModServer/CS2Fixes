import Source2Py
ADVPlayer   = Source2Py.ADVPlayer

import logging, inspect, traceback

logging.basicConfig(filename='tests/TestPlayerJump.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestPlayerJump]" + caller + str(message))
    log.info(msg=("[TestPlayerJump]" + caller + str(message)))
    pass

alog("START")
class TestPlayerJump:
    players_in_air = dict()
    last_grounded = dict()

    def OnPluginLoad(self):
        alog("")
        pass
    def OnGameFrame(self,
        simulating: bool,
        bFirstTick: bool,
        bLastTick: bool
        ):
        #alog("113 times a second, it reaches out.")

        for _slot in self.players_in_air:
            #note that just being in this list does not mean your actually in the air
            #just that airborn/ladder/grounded state is being tracked.
            player = ADVPlayer(_slot)
            grounded = player.IsOnGround()
            if not grounded:
                if player.IsOnLadder(): 
                    grounded = 1 #ladders count as ground too.
                '''
                if player.inWater():
                    figure out how to figure out.
                    grounded = 1
                '''

            if self.last_grounded.get(_slot, 1) != grounded:
                #if the last grounded state does not match what you are now, like
                #if you fall off a box, ladder, or roof... or jump :)
                self.last_grounded[_slot] = grounded
                if grounded:
                    ev = Source2Py.CreateFakeEvent("player_land", True)
                    if (ev):
                        geks = Source2Py.GameEventKeySymbol_t("userid")
                        ev.SetInt(geks, _slot)
                        Source2Py.FireFakeEvent(ev, True)
                else:
                    ev = Source2Py.CreateFakeEvent("player_airborn", True)
                    if (ev):
                        geks = Source2Py.GameEventKeySymbol_t("userid")
                        ev.SetInt(geks, _slot)
                        Source2Py.FireFakeEvent(ev, True)

        pass
    def OnPlayerJump(self, 
        _slot
        ):
        ''' "player_jump":dict({
            "userid":"playercontroller",
            }), '''

        if _slot in self.players_in_air and self.last_grounded.get(_slot, 1) == 0:
            if self.last_grounded.get(_slot, 1) == 0:
                self.last_grounded[_slot] = 1
                ev = Source2Py.CreateFakeEvent("player_land", True)
                if (ev):
                    geks = Source2Py.GameEventKeySymbol_t("userid")
                    ev.SetInt(geks, _slot)
                    Source2Py.FireFakeEvent(ev, True)

        alog("START")
        try:
            player = ADVPlayer(_slot)
            self.players_in_air[_slot] = True
            alog(player.GetName() + " jumped!")
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
    def OnPlayerLand(self, 
        _slot: int
        ):
        ''' "player_jump":dict({
            "userid":"playercontroller",
            }),'''
        alog("START")
        try:
            alog("_slot: " + str(_slot))
            player = ADVPlayer(_slot)
            alog(player.GetName() + " landed!")
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
    def OnPlayerAirborn(self, 
        _slot: int
        ):
        ''' "player_airborn":dict({
            "userid":"playercontroller",
            }),'''
        alog("START")
        try:
            alog("_slot: " + str(_slot))
            player = ADVPlayer(_slot)
            alog(player.GetName() + " is airborn!")
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
