import Source2Py

from tests import dumped_events

class TestLoadAnotherFile:
    def __init__(self):
        pass
    def OnPluginLoad(self):
        Source2Py.ServerPrint("[PY] TestLoadAnotherFile successfully loaded.")
        Source2Py.ServerPrint(str(dumped_events.dumped_events))
        pass