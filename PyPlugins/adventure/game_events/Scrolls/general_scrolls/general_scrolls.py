import Source2Py as s2
import logging, inspect, traceback
logging.basicConfig(filename='adventure/game_events/Scrolls/general_scrolls.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[general_scrolls]" + caller + str(message))
    log.info(msg=("[general_scrolls]" + caller + str(message)))
    pass

def build_scroll_template(scroll):
    scroll.func_kwargs.update({'scroll_test': 'build_scroll_template'})
    alog("build_scroll_template called!")
    pass


def activate_scroll_template(dispatch, *args, **kwargs):
    alog("ACTIVATE FIRED: SCROLL " + str(kwargs.items()))
    # nothing to do here, haven't thought up a template example.
    pass

def build_scroll_Normal(scroll):
    #if the scroll were special in some way, maybe affect it here
    pass

def activate_scroll_Normal(dispatch, *args, **kwargs):
    #if the scroll were special in some way, maybe affect it here
    pass