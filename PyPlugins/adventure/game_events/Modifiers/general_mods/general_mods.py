import Source2Py as s2
import logging, inspect, traceback
logging.basicConfig(filename='adventure/game_events/Modifiers/general_mods.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[general_mods]" + caller + str(message))
    log.info(msg=("[general_mods]" + caller + str(message)))
    pass

def build_modifier_template(scroll, *args, **kwargs):
    scroll.func_kwargs.update({'modifier_test': 'build_modifier_template'})
    alog("build modifier template fired")
    pass


def activate_modifier_template(dispatch, *args, **kwargs):
    alog("ACTIVATE FIRED: MODIFIER " + str(kwargs.items()))
