import Source2Py
import logging, inspect
import traceback

logging.basicConfig(filename='tests/TestExceptionTracebackPlugin.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestExceptionTracebackPlugin]" + caller + str(message))
    log.info(msg=("[TestExceptionTracebackPlugin]" + caller + str(message)))
    pass

class TestExceptionTracebackPlugin:
    def __init__(self):
        pass
    def OnPluginLoad(self):
        Source2Py.ServerPrint("[PY] TestExceptionTracebackPlugin successfully loaded.")
        try:
            alog("try:", True)
            #raise ValueError('test')  #it worked but i'm too tired to figure out how to call pybind11::PyErr_Clear() in cpp
        except ValueError as ve:
            alog("except:", True)
            alog(traceback.format_exc(), True)
            alog(str(ve), True)
            alog("cleared test python exception")
            pass

