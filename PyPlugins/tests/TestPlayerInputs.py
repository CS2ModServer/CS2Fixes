import Source2Py
ADVPlayer   = Source2Py.ADVPlayer

import logging, inspect, traceback

logging.basicConfig(filename='tests/TestPlayerInputs.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestPlayerInputs]" + caller + str(message))
    log.info(msg=("[TestPlayerInputs]" + caller + str(message)))
    pass

alog("START")
class TestPlayerInputs:
    def OnPluginLoad(self):
        alog("")
        pass
    def _checkButtonStates(self, _slot):
        try:
            alog("_slot: " + str(_slot))
            player = ADVPlayer(_slot)
            if (player.IsValid()):
                bstates = player.GetButtonStates()

                #incomplete list, missing weapon select 1,2,3,4 (eg)
                IN_ATTACK =     1 << 0
                alog(str(bstates & IN_ATTACK) + " IN_ATTACK")
                IN_JUMP =       1 << 1
                alog(str(bstates & IN_JUMP) + " IN_JUMP")
                IN_DUCK =       1 << 2
                alog(str(bstates & IN_DUCK) + " IN_DUCK")
                IN_FORWARD =    1 << 3
                alog(str(bstates & IN_FORWARD) + " IN_FORWARD")
                IN_BACKWARD =   1 << 4
                alog(str(bstates & IN_BACKWARD) + " IN_BACKWARD")
                IN_USE =        1 << 5
                alog(str(bstates & IN_USE) + " IN_USE")

                #6
                #7
                #8

                IN_MOVELEFT =   1 << 9
                alog(str(bstates & IN_MOVELEFT) + " IN_MOVELEFT")
                IN_MOVERIGHT =  1 << 10
                alog(str(bstates & IN_MOVERIGHT) + " IN_MOVERIGHT")
                IN_ATTACK2 =    1 << 11
                alog(str(bstates & IN_ATTACK2) + " IN_ATTACK2")

                #12

                IN_RELOAD =     1 << 13
                alog(str(bstates & IN_RELOAD) + " IN_RELOAD")

                #14
                #15

                IN_SPRINT =     1 << 16
                alog(str(bstates & IN_SPRINT) + " IN_SPRINT")

                IN_SCORE = 0x200000000
                alog(str(bstates & IN_SCORE) + " IN_SCORE")
                IN_LOOK_AT_WEAPON = 0x800000000
                alog(str(bstates & IN_LOOK_AT_WEAPON) + " IN_LOOK_AT_WEAPON")

        except Exception as e:
            alog(e)
            alog(traceback.format_exc())        
    def OnClientCommand(self,
        _slot: int,
        _cmd: str):
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            self._checkButtonStates(self, _slot)
            alog(player.GetName() + " used a ClientCommand: {0}".format(_cmd))
        pass
    def OnClientAbility1(self,
        _slot: int
        ):
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            self._checkButtonStates(self, _slot)
            alog(player.GetName() + " used Ability1!")
        pass
    def OnClientAbility2(self,
        _slot: int
        ):
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            self._checkButtonStates(self, _slot)
            alog(player.GetName() + " used Ability2!")
        pass
    def OnClientUltimate(self,
        _slot: int
        ):
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            self._checkButtonStates(self, _slot)
            alog(player.GetName() + " used Ultimate!")
        pass
    def OnPlayerJump(self, 
        _slot: int
        ):
        alog("START")
        self._checkButtonStates(self, _slot)
        alog("END")
        pass


