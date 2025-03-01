import Source2Py
import logging, inspect
import traceback

#logging.basicConfig(filename='SampleEvents.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
logging.basicConfig(filename='TestPlayerHurt.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging


def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestPlayerHurt]" + caller + str(message))
    log.info(msg=("[TestPlayerHurt]" + caller + str(message)))
    pass

alog("START")
class TestPlayerHurt:
    '''"player_hurt":dict({
            "userid":           GetInt //they say it's "playercontroller" but it isn't, its the playerindex
            "userid_pawn":      Get??? // "strict_ehandle",
            "attacker":         GetInt //they say it's "playercontroller" but it isn't, its the playerindex
            "attacker_pawn":    Get??? // "strict_ehandle",
            "health":           GetInt // "byte",
            "armor":            GetInt // "byte",
            "weapon":           GetStr // name of the weapon used "string",
            "dmg_health":       GetInt // health damage dealt with the player_hurt "short",
            "dmg_armor":        GetInt // armor damage dealt with the player_hurt "byte",
            "hitgroup":         GetInt // 1=head, 2=body_upper, 3=body_lower, 4=arm_left, 5=arm_right, 6=leg_left, 7=leg_right
            }),'''
    def OnPluginLoad(self):
        alog("successfully loaded")
        #Source2Py.ServerPrint("OnLoad - TestPlayerHurt successfully loaded")
        pass 

    def _testGetPawnEHandle(self, event):
        geks = Source2Py.GameEventKeySymbol_t("attacker_pawn")
        peHandle = event.GetPawnEHandle(geks) #strict_ehandle??
        alog("peHandle type: " + str(type(peHandle)))

        ent = peHandle.TestHealthCEH()
        pass
    def _testGetPlayerSlot(self, event):
        #using GetInt("userid") or //"attacker"
        geks = Source2Py.GameEventKeySymbol_t("userid")
        vslot = event.GetPlayerSlot(geks) #GetPlayerSlot works because the constructor expects a single integer input and the event data contained in "userid" and "attacker" is the playerindex on the server.
        vslot_type = str(type(vslot))
        Source2Py.ServerPrint("event.GetPlayerSlot('userid'): type(" + vslot_type + ")")
        Source2Py.ServerPrint("event.GetPlayerSlot('userid'): " + str(vslot.Get()))
        Source2Py.ServerPrint("vslot.TestGetHealth(): " + str(vslot.TestGetHealth()))

        geks = Source2Py.GameEventKeySymbol_t("attacker")
        aslot = event.GetPlayerSlot(geks) #GetPlayerSlot works because the constructor expects a single integer input and the event data contained in "userid" and "attacker" is the playerindex on the server.
        aslot_type = str(type(aslot))
        Source2Py.ServerPrint("event.GetPlayerSlot('userid'): type(" + aslot_type + ")")
        Source2Py.ServerPrint("event.GetPlayerSlot('userid'): " + str(aslot.Get()))
        Source2Py.ServerPrint("aslot.TestGetHealth(): " + str(aslot.TestGetHealth()))

        geks = Source2Py.GameEventKeySymbol_t("dmg_health")
        damage = event.GetInt(geks, 0)
        if (damage>0):
            heal = int(damage / 5)+1
            if (aslot.TestAddHealth(heal)):
                Source2Py.ServerPrint("attacker healed: " + str(heal))
                Source2Py.ServerPrint("aslot.TestGetHealth(): " + str(aslot.TestGetHealth()))
            else:
                Source2Py.ServerPrint("aslot.TestAddHealth(" + str(heal) + ") -failed-")
        pass
    def OnPlayerHurt(self, event):
        alog("START")

        try:
            #self._testGetPawnEHandle(self, event)
            self._testGetPlayerSlot(self, event)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())

        #aentity.TestHealth()

        #geks = Source2Py.GameEventKeySymbol_t("userid_pawn")
        #vpawn = event.GetPlayerPawn(geks)
        #vpawn.TestHealth()

        Source2Py.ServerPrint("OnPlayerHurt - TestPlayerHurt END")
        pass

alog("END")


