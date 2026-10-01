import Source2Py as s2
ADVPlayer = s2.ADVPlayer

from adventure.game_events import game_events
Event = game_events.Event

import logging, inspect, traceback, os, sys, importlib

logging.basicConfig(filename='adventure/trigger_events.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    s2.ServerPrint("[trigger_events]" + caller + str(message))
    log.info(msg=("[trigger_events]" + caller + str(message)))
    pass

ModifierTemplate = {
    'name': 'Modifier template',
    'desc': 'Modifier description template',
    'id': 0,  # None is an actual value don't use.
    'type': 'template',
    'func_activate': None,
    'func_build': None,  # called by modifier to apply changes to tech func.
    'func_kwargs': {},
    'func_tuples': [],
}
TechniqueTemplate = {
    'name': 'Technique template',
    'desc': 'Technique template',
    'id': 0,  # None is an actual value, overwrite or else.
    'type': 'template',
    'func_activate': None,
    'func_build': None,  # called by modifier to apply changes to tech func.
    'func_kwargs': {},
    'func_tuples': [],
}
ScrollTemplate = {
    'name': 'Scroll template',
    'desc': 'Scroll template',
    'id': 0,  # None is an actual value, overwrite or else.
    'type': 'template',
    'func_activate': None,
    'func_build': None,  # called by modifier to apply changes to tech func.
    'func_kwargs': {},
    'func_tuples': [],
    'default_technique_id': 0,
    'technique': None, # TechniqueTemplate   was used previously, probably should not be.
    'max_modifiers': 1, # 0-N accepted
    'default_technique_lock': None,
    'default_modifier_locks': {},
    'default_modifier_ids': {},
    'modifier_dict': {},
}
try:

    class Modifier(object):
        def __init__(self, entry):
            self.modifier_dict = None
            self.func_build = None
            self.func_activate = None
            self.func_kwargs = None
            self.func_tuples = [] # list of tuples [ (), () ]

            for key, val in ModifierTemplate.items():
                self.__dict__.update({key: entry.get(key, val)})
                # self[key] = entry.get(key, val)

        def __getitem__(self, key, default=None):
            return getattr(self, key, default)

        def get(self, key, default=None):
            try:
                return self[key]
            except KeyError:
                return default

        def __setitem__(self, key, value):
            self.__dict__[key] = value

        def build(self, scroll):
            print('Modifier.Build fired')
            if self.func_build is not None:
                self.func_build(scroll)

            self.func_tuples.clear()
            tup = tuple((self.func_activate, self.func_kwargs))
            self.func_tuples.append(tup)
            pass
    m = Modifier
    class ModifierContainer(object):
        unprocessed_string = ""  # raw from file
        unprocessed_list = []  # split into blocks
        list_all = []  # each block processed into list
        dict_by_id = {}  # list iterated and added to dict by id

        def __init__(self, input_data):
            self.unprocessed_string = input_data
            self.unprocessed_list = self.unprocessed_string.split("#BREAK#\n")

            for single_entry in self.unprocessed_list:
                if not single_entry.strip():
                    continue

                safe = True
                while safe:
                    try:
                        self.list_all.append(eval(single_entry))
                        safe = False
                    except SyntaxError as e:
                        for row in single_entry.split("\n"):
                            if row.strip() and not row.strip().startswith("#"):
                                safe = False
                                break
                        alog(e)
                        alog(traceback.format_exc())
                        #print("1", e, "\n", single_entry)
                        pass
                    except AttributeError as e:
                        alog(e)
                        alog(traceback.format_exc())
                        #print("AttributeError:", e, "\n", single_entry)
                        break

            for single_data in self.list_all:
                self.dict_by_id.update({single_data.id: single_data})
            pass

        def __getitem__(self, key, default=None):
            return getattr(self, key, default)

        def get(self, key, default=None):
            try:
                return self[key]
            except KeyError:
                return default

        def __setitem__(self, key, value):
            self.__dict__[key] = value

except Exception as e:
    alog(e)
    alog(traceback.format_exc())
try:
    class Technique(object):
        def __init__(self, entry):
            self.func_activate = None
            self.func_build = None
            self.func_kwargs = None
            self.func_tuples = []
            for key, val in TechniqueTemplate.items():
                self.__dict__.update({key: entry.get(key, val)})
            pass

        def __getitem__(self, key, default=None):
            return getattr(self, key, default)

        def get(self, key, default=None):
            try:
                return self[key]
            except KeyError:
                return default

        def __setitem__(self, key, value):
            self.__dict__[key] = value

        def build(self, scroll):
            if self.func_build is not None:
                self.func_build(scroll)

            self.func_tuples.clear()
            tup = tuple((self.func_activate, self.func_kwargs))
            self.func_tuples.append(tup)
            # self.func_tuples.append(tuple((self.func_activate, self.func_kwargs)))
            pass
    t = Technique
    class TechniqueContainer(object):
        unprocessed_string = "" # raw from file
        unprocessed_list = [] # split into blocks
        list_all = [] # each block processed into list
        dict_by_id = {} # list iterated and added to dict by id

        def __init__(self, input_data):
            self.unprocessed_string = input_data
            self.unprocessed_list = self.unprocessed_string.split("#BREAK#\n")
            
            for single_entry in self.unprocessed_list:
                if not single_entry.strip(): #empty line
                    continue
                
                safe = True
                while safe:
                    try:
                        self.list_all.append(eval(single_entry))
                        safe = False
                    except SyntaxError as e:
                        for row in single_entry.split("\n"):
                            if row.strip() and not row.strip().startswith("#"):
                                safe = False
                                break

                        print("SyntaxError", e, "\n", single_entry)
                        pass
                    except AttributeError as e:
                        print("AttributeError:", e, "\n", single_entry)
                        break

            for single_data in self.list_all:
                self.dict_by_id.update({single_data.id: single_data})
            pass
            
        def __getitem__(self, key, default=None):
            return getattr(self, key, default)

        def get(self, key, default=None):
            try:
                return self[key]
            except KeyError:
                return default

        def __setitem__(self, key, value):
            self.__dict__[key] = value

except Exception as e:
    alog(e)
    alog(traceback.format_exc())

"""
Scroll
1: Build scroll
* retrieve technique and modifier data and apply appropriately
* process modifiers' build calls first to build kwargs dict the technique/scroll will use
* add any modifier activate calls to the scroll' func_tuple list([tuple(activate, kwargs), ])
* process technique second using it's build call and any applicable additions to
** the kwargs dict from modifiers/scroll
2: Activate scroll
* scroll ready to be used and will have it's dispatch added to the players dispatch dict
** active type scrolls would be used by a player pressing a key/button causing 
*** an event the scroll is assigned to be called.
** passive type scrolls add dispatch events such as ...
*** on_spawn: 'when player spawns fire any that are assigned to trigger in this way.'
*** on_death, on_jump, on_land, on_attack, on_block, etc.  
"""
# todo cooldown for scrolls unsure of approach to take, will figure it out when I get there haha.

class Scroll(object):
    def __init__(self, entry):
        self.name = None
        self.desc = None
        self.id = None
        self.type = None
        self.func_activate = None
        self.func_build = None
        self.func_kwargs = None
        self.func_tuples = []
        # func_tuples must contain tuples that are (func_activate, func_kwargs)
        # func_tuples is a list and may contain multiple in the event a modifier has some special function to call
        self.default_technique_lock = None
        self.default_technique_id = None
        self.technique = None
        self.default_modifier_locks = {}
        self.default_modifier_ids = {}
        self.max_modifiers = None
        self.modifier_dict = {}
        self.owner = -1

        for key, val in ScrollTemplate.items():
            self.__dict__.update({key: entry.get(key, val)})

    def __getitem__(self, key, default=None):
        return getattr(self, key, default)

    def get(self, key, default=None):
        try:
            return self[key]
        except KeyError:
            return default

    def __setitem__(self, key, value):
            self.__dict__[key] = value

    def build(self, event_manager, 
        # category="bind", 
        action_map="use_ability1"):

        if self.default_technique_id is None:
            alog("default_technique_id is None - Failed.")
            return False

        self.technique = event_manager.TM.dict_by_id[ self.default_technique_id ]
        if self.technique is None:
            alog("Scroll had no technique it was None! - Failed.")
            return False

        self.func_tuples.clear()

        for key in self.default_modifier_ids.keys():
            mod = event_manager.MM.dict_by_id[key]
            self.modifier_dict.update({key: mod})
        for key in self.modifier_dict.keys():
            self.modifier_dict[key].build(self)

            if self.modifier_dict[key].func_tuples is not None:
                self.func_tuples.extend(self.modifier_dict[key].func_tuples)

        ''' With all modifiers having affected the Scroll, build the technique '''
        self.technique.build(self)

        if self.technique.func_tuples is not None:
            self.func_tuples.extend(self.technique.func_tuples)

        ''' Reference to self needed here as it's a function outside this class. '''
        if self.func_build is not None:
            self.func_build(self)

        self.func_tuples.append(tuple((self.func_activate, self.func_kwargs)))
        temp = SingleDispatch(
            # category="bind",
            # action_map='use_ability1',
            # category=category,
            action_map=action_map,
            scroll_self=self,
            func_activate=self.activate,
            owner=self.owner)
        event_manager.DM.addDispatch(temp)

        return True

    def activate(self, dispatch):
        for (func_activate, func_kwargs) in self.func_tuples:
            func_activate(self, kwargs=func_kwargs)
        pass

s = Scroll

class ScrollContainer(object):
    unprocessed_string = ""  # raw from file
    unprocessed_list = []  # split into blocks
    list_all = []  # each block processed into list
    dict_by_id = {}  # list iterated and added to dict by id
    Scroll = Scroll
    s = Scroll
    def __init__(self, input_data):
        self.unprocessed_string = input_data
        self.unprocessed_list = self.unprocessed_string.split("#BREAK#\n")

        for single_entry in self.unprocessed_list:
            if not single_entry.strip():
                continue

            safe = True
            while safe:
                try:
                    self.list_all.append(eval(single_entry))
                    safe = False
                except SyntaxError as e:
                    for row in single_entry.split("\n"):
                        if row.strip() and not row.strip().startswith("#"):
                            safe = False
                            break

                    print("SyntaxError", e, "\n", single_entry)
                    pass
                except AttributeError as e:
                    print("AttributeError:", e, "\n", single_entry)
                    break

        for single_data in self.list_all:
            self.dict_by_id.update({single_data.id: single_data})
        pass

    def __getitem__(self, key, default=None):
        return getattr(self, key, default)

    def get(self, key, default=None):
        try:
            return self[key]
        except KeyError:
            return default

    def __setitem__(self, key, value):
        self.__dict__[key] = value

class SingleDispatch(object):
    def __init__(self, *args, **kwargs):
        self.scroll_self = kwargs.get('scroll_self', None)
        # self.category = kwargs.get('category', None)
        self.action_map = kwargs.get('action_map', None)
        self.func_activate = kwargs.get('func_activate', None)
        self.owner         = kwargs.get('owner', -1)
        pass

    def __getitem__(self, key, default=None):
        return getattr(self, key, default)

    def get(self, key, default=None):
        try:
            return self[key]
        except KeyError:
            return default

    def __setitem__(self, key, value):
        self.__dict__[key] = value

    def activate(self):
        print("SingleDispatch.activate()")
        self.func_activate(self)

class DispatchManager(object):
    dict_by_bind = {} # dict {py_id: {bind_id: func_tuples}}

    def __init__(self, _trigger_manager):
        self.trigger_manager = _trigger_manager
        pass

    def __getitem__(self, key, default=None):
        return getattr(self, key, default)

    def get(self, key, default=None):
        try:
            return self[key]
        except KeyError:
            return default

    def __setitem__(self, key, value):
        self.__dict__[key] = value

    def addDispatch(self, dispatch):
        try:
            # if dispatch.category is 'bind':
            dispatch.owner = dispatch.scroll_self.owner
            if dispatch.owner not in self.dict_by_bind:
                self.dict_by_bind[dispatch.owner] = {}

            am = dispatch.action_map
            self.dict_by_bind[dispatch.owner].update({am: dispatch})
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
    @classmethod
    def fireBind(self, d) -> bool:
        try:
            caller = d["userid"]
            request = d["event_name"]
            if self.hasBind(caller, request):
                self.dict_by_bind[caller][request].activate()
                return True
            else:
                return False
            
            #if request in self.dict_by_bind[caller].keys():
            #    self.dict_by_bind[caller][request].activate()
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
    @classmethod
    def hasBind(self, caller, request):
        if caller not in self.dict_by_bind:
            return False
        if request not in self.dict_by_bind[caller]:
            return False
        return True

class TriggerEventManager(object):
    modifiers_file = None
    techniques_file = None
    scrolls_file = None
    MM = None
    TM = None
    SM = None
    DM = None
    @classmethod
    def __init__(self, 
        binds: [], 
        events: []
        ):
        try:
            self.DM = DispatchManager(self)
            self.load()
            # self.post_load()
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())

        self.event_triggers = dict()
        #for t in events:
        #    self.registerEventTriggers(t)

        self.cooldowns = dict()
        pass
    @classmethod
    def load(self):
        self.realpath = os.path.realpath(__file__)
        self.realdirectory = os.path.dirname(self.realpath)
        
        self.__load_to_global(['Modifiers', 'Techniques', 'Scrolls'])

        self.modifiers_file = open(self.realdirectory + "/Modifiers/Modifiers.py", "r").read()
        self.MM = ModifierContainer(self.modifiers_file)

        self.techniques_file = open(self.realdirectory + "/Techniques/Techniques.py", "r").read()
        self.TM = TechniqueContainer(self.techniques_file)

        self.scrolls_file = open(self.realdirectory + "/Scrolls/Scrolls.py", "r").read()
        self.SM = ScrollContainer(self.scrolls_file)
    # @classmethod
    # def post_load(self):
    #     try:
    #         self.SM.dict_by_id[0].build(self)
    #     except Exception as e:
    #         alog(e)
    #         alog(traceback.format_exc())
    @classmethod
    def __add_to_path(self, target):
        if not sys.path.count(target) > 0:
            sys.path.append(target)
    @classmethod
    def __load_to_global(self, folders):
        scripts = os.path.dirname(os.path.realpath(__file__))
        for folder in folders:
            tempdir = str(scripts + "\\" + folder)
            print(tempdir)
            self.__add_to_path(tempdir)

            #list comprehension to grab name only for directories within the folders noted above.
            #requires python 3.7, don't forget.
            for sub in [entry.name for entry in os.scandir(tempdir) if os.path.isdir(entry)]:
                if sub.startswith('__pycache__'):
                    continue

                print(tempdir + "\\" + sub)
                self.__add_to_path(tempdir + "\\" + sub)

                # each sub in each folder has a *.py file that is the same name as the directory it resides in hence
                # hence sub.sub or rather /general_mods/general_mods.py
                # any other file within those subs will only be included if the primarily loaded 'sub' does so.
                # 'include general_mods.vibrate' is okay. 
                ##globals()[sub] = importlib.import_module('' + sub, package='{0}.{0}.py'.format(sub))
                package = str("{s}.py".format(s=sub))
                # alog("package= " + str(package))
                globals()[sub] = importlib.import_module('' + sub, package)
#    @classmethod
#    def registerEventTriggers(self, trigger: str):
#        empty_dict = dict()
#        self.event_triggers[trigger] = empty_dict
#        pass
#    @classmethod
#    def _fireTrigger(self, slot, t, event_dict):
#        current = s2.GetTickCount()
#        tname = t.get("name", None)
#        if tname is not None:
#            time_used = self.cooldowns[slot].get(tname, 0)
#            if t.cooldown < current - time_used:
#                alog("still on cooldown")
#                return False
#
#        return True
#    @classmethod
#    def fireEventTrigger(self, 
#        trigger: str, 
#        slot: int, 
#        event_dict: Event):
#        p = ADVPlayer(slot)
#        if not p.IsValid():
#            alog("p invalid")
#            return False
#
#        for t in self.event_triggers[trigger].get(slot, set()).slice():
#            alog("{t} event found for player {p}".format(t=t, p=slot))
#            return self._fireTrigger(slot, t, event_dict)
#        pass
#    @classmethod
#    def fireBindTrigger():
#        #do later
#        pass
