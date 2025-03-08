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

    def OnGameFrame(self, 
        simulating: bool,
        bFirstTick: bool,
        bLastTick: bool
        ):

        pass
    def OnPlayer______(self, 
        _slot: int
        ):
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


'''button_flags = 
{
    "IN_NONE":          0x0,
    "IN_ALL":           0xffffffffffffffff,
    "IN_ATTACK":        0x1,
    "IN_JUMP":          0x2,
    "IN_DUCK":          0x4,
    "IN_FORWARD":       0x8,
    "IN_BACK":          0x10,
    "IN_USE":           0x20,
    "IN_TURNLEFT":      0x80,
    "IN_TURNRIGHT":     0x100,
    "IN_MOVELEFT":      0x200,
    "IN_MOVERIGHT":     0x400,
    "IN_ATTACK2":       0x800,
    "IN_RELOAD":        0x2000,
    "IN_SPEED":         0x10000,
    "IN_JOYAUTOSPRINT": 0x20000,

}'''

