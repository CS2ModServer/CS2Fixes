import Source2Py
import logging, inspect

logging.basicConfig(filename='tests/TestImportsPlugin.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestImportsPlugin]" + caller + str(message))
    log.info(msg=("[TestImportsPlugin]" + caller + str(message)))
    pass

class TestImportsPlugin:
    def __init__(self):
        pass
    def OnPluginLoad(self):
        Source2Py.ServerPrint("[PY] TestImportsPlugin successfully loaded.")
        alog("Message from alog, written to logfile, callername=true", True)

