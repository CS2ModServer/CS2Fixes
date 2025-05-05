import Source2Py

import logging, inspect, traceback
ADVPlayer = Source2Py.ADVPlayer

logging.basicConfig(filename='tests/TestOnAllPluginsLoaded.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestOnAllPluginsLoaded]" + caller + str(message))
    log.info(msg=("[TestOnAllPluginsLoaded]" + caller + str(message)))
    pass

alog("START")
class TestOnAllPluginsLoaded:
    ability_name = "fireball"
    def OnPluginLoad(self):
        alog("")
        pass
    def OnAllPluginsLoaded(self, message: str):
        alog("START")
        try:
            alog(message)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
    def _teststdstring(self, wrap_func):
        try:
            temp = str("contents")
            alog("TRY1")
            alog(str(temp))
            alog(str(type(temp)))
            alog(str(wrap_func))
            alog(str(type(wrap_func)))
            wrap_func(temp)
            alog("TRY2")
        except Exception as e:
            alog("EXCEPT")
            alog(e)
            alog(traceback.format_exc())
        alog("END")
        pass
    def _change_my_item(
            self, 
            slot: int,  
            newitem: str):
        try:
            alog("TRY")
            count = Source2Py.GetTickCount()
            pPlayer = ADVPlayer(slot)
            pPlayer._items_last_update = count
            alog("_change_my_item called at count " + str(count) + " olditem= " + str(self.ability_name) + " newitem= " + str(newitem))
            self.ability_name = newitem
        except Exception as e:
            alog("EXCEPT")
            alog(e)
            alog(traceback.format_exc())
        pass
    def OnClientAbility1(
            self, 
            slot: int):
        try:
            alog("TRY")
            self._change_my_item(self, slot, "asdfoiud")
            alog("ability1 pressed")
        except Exception as e:
            alog("EXCEPT")
            alog(e)
            alog(traceback.format_exc())
        pass



