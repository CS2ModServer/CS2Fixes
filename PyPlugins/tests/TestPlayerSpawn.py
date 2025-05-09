import Source2Py
ADVPlayer   = Source2Py.ADVPlayer

import logging, inspect

from random import randint

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='tests/TestPlayerSpawn.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestPlayerSpawn]" + caller + str(message))
    log.info(msg=("[TestPlayerSpawn]" + caller + str(message)))
    pass

alog("START")
class TestPlayerSpawn:
    ''' note that python does not need typing in it's method declarations but here it is being used 
        as a reminder of the incoming type from CPP.
    '''
    def OnPluginLoad(self):
        alog("")
        pass 
    def OnPlayerSpawn(self,
        _slot: int
        ):
        #manipulating the pawn here will happen before the player receives their new pawn which will
        #overwrite the changes. :(
        alog("")
        pass
    def OnPlayerSpawn_post(self,
        _slot: int
        ):
        alog("START")
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            bonus_hp = randint(10, 50)
            player.AddHealth(bonus_hp)
        alog("END")
        pass