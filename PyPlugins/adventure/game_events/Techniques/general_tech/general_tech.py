import Source2Py as s2
import Source2Py

ADVPlayer = s2.ADVPlayer

import logging, inspect, traceback
logging.basicConfig(filename='adventure/game_events/Techniques/general_tech.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[general_tech]" + caller + str(message))
    log.info(msg=( "[general_tech]" + caller + str(message)))
    pass

# TEMPLATE - START #
def build_technique_template(scroll) -> bool:
    alog("build technique template fired")
    scroll.func_kwargs.update({'technique_test': 'build_technique_template'})
    for k, v in scroll.modifier_dict.items():
        alog("mod slot {0} has modifier number {1} which is {2}".format(k, v.id, v.name))
    return True
def activate_technique_template(dispatch, *args, **kwargs) -> bool:
    alog("ACTIVATE FIRED: TEMPLATE " + str(kwargs.items()))
    return True
# TEMPLATE - END #


# Explode - START #
def build_technique_Explode(scroll) -> bool:

    return True

def activate_technique_Explode(dispatch, *args, **kwargs) -> bool:
    """
    Available fields in a dispatch are: 
    dispatch.scroll_self
    dispatch.category
    dispatch.action_map
    dispatch.func_activate
    dispatch.owner
    """

    alog(" Explode ".center(60, "="))
    for k, v in dispatch.__dict__.items():
        alog(str(k).ljust(25) + " | " + str(v))

    userid = dispatch.owner
    # player = ADVPlayer(userid) 

    # if not player.IsValid():
    #     alog("player.IsValid()".ljust(25) + " | " + "False")
    #     return False

    # pawn = player.pawn
    # if not pawn:
    #     alog("pawn.IsValid()".ljust(25) + " | " + "False")
    #     return False

    distance = 400.0
    ignoreList = [userid,]

    #return a list of playerids that were in range.
    pList = s2.GetPlayersNearPlayerID_list(userid, distance, ignoreList)
    alog("players in {r} range".format(r=distance).ljust(25) + " | " + str(pList))

    pDict = s2.GetPlayersNearPlayerID_dict(userid, distance, ignoreList)
    for k, v in pDict:
        alog("Distance from Player {n} to you.".format(n=ADVPlayer(k).name).ljust(25) + " | " + str(v))






    return True
# Explode - END #

