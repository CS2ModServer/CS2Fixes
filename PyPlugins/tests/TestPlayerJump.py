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
        alog("loaded!")
        pass
    def pre_player_jump(self,
        event
        ):
        ''' "player_jump":dict({
            "userid":"playercontroller",
            }), '''
        pass
    def player_jump(self, 
        event
        ):
        ''' "player_jump":dict({
            "userid":"playercontroller",
            }), '''
        try:
            player = ADVPlayer(slot)
            if (player.IsValid()):
                name = player.GetName()
                if (name):
                    alog(name + " player_jump!")
                else:
                    alog("player.GetName() returned nullptr (player_jump!)")
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def post_player_jump(self,
        event
        ):
        ''' "player_jump":dict({
            "userid":"playercontroller",
            }), '''
        pass
    
