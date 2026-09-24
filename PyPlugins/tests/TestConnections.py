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
    def OnPluginLoaded(self):
        alog("=============== TestConnections.py  ==============")
        try:
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def player_connect(self, 
        event: GameEvent
        ):
        for k,v in d.items():
            alog(str(k).ljust(14) + " | " + str(v))
        pass
    def player_disconnect(self,
        event: GameEvent
        ):
        pass
    def player_put_in_server(self,
        event: GameEvent
        ):
        pass