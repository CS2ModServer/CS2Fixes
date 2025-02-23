import Source2Py

class TestLoadPlugin:
    def __init__(self):
        pass
    def OnPluginLoad(self):
        Source2Py.ServerPrint("[PY] TestLoadPlugin successfully loaded.")
        pass