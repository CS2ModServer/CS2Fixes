import Source2Py
ADVPlayer   = Source2Py.ADVPlayer

import logging, inspect, traceback

logging.basicConfig(filename='tests/TestEffects.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestEffects]" + caller + str(message))
    log.info(msg=("[TestEffects]" + caller + str(message)))
    pass

alog("START")
class TestEffects:
    def OnPluginLoad(self):
        alog("")
        pass
    def OnClientAbility1(self,
        _slot: int
        ):
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            bES = player.EmitSound(
                "zr.amb.base",   #const char* _pszSound, 
                100,  #int _nPitch     = 100, 
                1.0,  #float _flVolume =   1.0f, 
                0.0); #float _flDelay  =   0.0f
            alog(player.GetName() + " used Ability1! nearby:" + str(bES))
        pass
    def OnClientAbility2(self,
        _slot: int
        ):
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            bESP = player.EmitSoundPersonal(
                "zr.amb.scream",   # const char* _pszSound,
                100,  # int _nPitch,
                1.0); # float _flVolume);
            alog(player.GetName() + " used Ability2! personal:" + str(bESP))
        pass
    def OnClientUltimate(self,
        _slot: int
        ):
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            bESA = player.EmitSoundAll(
                "zr.amb.zombie_die",   # const char* _pszSound,
                100,  #int _nPitch,
                1.0); #float _flVolume);
            alog(player.GetName() + " used Ultimate! all:" + str(bESA))
        pass
    def OnPlayerJump(self, 
        _slot: int
        ):
        alog("START")
        player = ADVPlayer(_slot)
        if (player.IsValid()):
            bES = player.EmitSound(
                "zr.amb.zombie_pain",   #const char* _pszSound, 
                100,  #int _nPitch     = 100, 
                1.0,  #float _flVolume =   1.0f, 
                0.0); #float _flDelay  =   0.0f
            alog(player.GetName() + " Jumped! mario:" + str(bES))
        alog("END")
        pass


