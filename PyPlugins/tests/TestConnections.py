import Source2Py
import logging, inspect
import traceback

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='tests/TestConnections.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging


def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestConnections]" + caller + str(message))
    log.info(msg=("[TestConnections]" + caller + str(message)))
    pass

alog("START")
class TestConnections:
    ''' note that python does not need typing in it's method declarations but here it is being used 
        as a reminder of the incoming type from CPP.
    '''
    def OnClientConnected(self, 
        _slot: int, 
        _name: str, 
        _xuid: int, 
        _networkID: str, 
        _ipAddress: str, 
        _fake: bool):
        #first brush when player connects, no name available yet.
        alog("START")
        try:
            alog("slot: " + str(_slot))
            alog(_name + " connected")
            alog("xuid: " + str(_xuid))
            alog("networkID: " + str(_networkID))
            alog("ipAddress: " + str(_ipAddress))
            alog("fake: " + str(_fake))
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
    def OnClientPutInServer(self, 
        _slot: int, 
        _name: str, 
        _type: int, 
        # type values could be:
        # 0 - player
        # 1 - fake player (bot)
        # 2 - unknown
        _xuid: int):
        alog("START")
        try:
            alog("slot: " + str(_slot))
            alog(_name + " put in server")
            alog("type: " + str(_type))
            alog("xuid: " + str(_xuid))
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
    def OnClientDisconnect(self, 
        _slot: int, 
        _reason: int, 
        _name: str, 
        _xuid: int, 
        _networkID: str):
        alog("START")
        try:
            alog("slot: " + str(_slot))
            alog("reason: " + str(_reason))
            alog(_name + " disconnected")
            alog("xuid: " + str(_xuid))
            alog("networkID: " + str(_networkID))
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass

