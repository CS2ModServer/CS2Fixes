import Source2Py

Source2Py.ServerPrint("START of TestPlayerHurt.py")
class TestPlayerHurt:
    def __init__(self):
        pass
    def OnPluginLoad(self):
        Source2Py.ServerPrint("OnLoad - TestPlayerHurt successfully loaded")
        pass
    def OnPlayerHurt(self, event):
        Source2Py.ServerPrint("OnPlayerHurt - PlayerHurt successfully Fired")
        Source2Py.ServerPrint("test")
        aHealth = event.GetPlayerController("attacker").GetHealth()
        vHealth = event.GetPlayerController("userid").GetHealth()

        Source2Py.ServerPrint("attacker health: " + str(aHealth) + ", victim health: " + str(vHealth))
        pass
        #type_event = str(type(event))
        #Source2Py.ServerPrint("test0 - type_event: " + type_event)

        #geks = Source2Py.GameEventKeySymbol_t("attacker")
        #type_geks = str(type(geks))
        #Source2Py.ServerPrint("test1 - type_geks: " + type_geks)
        #
        #centityinstance = event.GetPlayerController(geks)
        #Source2Py.ServerPrint("test2 - type_centityinstance: " + str(type(centityinstance)))
        #
        #ccsPC = Source2Py.CEntityInstance_to_CCSPlayerController(centityinstance)
        #
        #health = ccsPC.GetHealth()
        #Source2Py.ServerPrint("test2 - health: " + health)
        #ccsPC.SetHealth(int(150))
        #health = ccsPC.GetHealth()
        #Source2Py.ServerPrint("test2 - health: " + health)
        #
        pass

Source2Py.ServerPrint("END of TestPlayerHurt.py")


