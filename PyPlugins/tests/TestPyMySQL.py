import Source2Py
GameEvent = Source2Py.GameEvent
GameEventKeySymbol_t = Source2Py.GameEventKeySymbol_t
ADVPlayer = Source2Py.ADVPlayer

import logging, inspect, traceback
import pymysql

TestDBConfig = {
    'host':     'localhost',
    'port':     3306,
    'user':     'TestPyMySQL',
    'password': 'TestPassword',
    'database': 'testkilllog', #mysql uses lowercase only, python cares about case difference
    'table':    'testtable'
}

logging.basicConfig(filename='tests/TestPyMySQL.log', encoding='utf-8', level=logging.DEBUG, format='[%(asctime)s]%(message)s', datefmt='%H:%M:%S')
log = logging

def alog(message: str, callername: bool = True):
    caller = str("")
    if (callername):
        caller = "[" + str(inspect.stack()[1].function) + "] "
    Source2Py.ServerPrint("[TestPyMySQL]" + caller + str(message))
    log.info(msg=("[TestPyMySQL]" + caller + str(message)))
    pass

default_player_dict = dict({
    "loaded":   False,
    "name":     "default",
    "steamid":  "-1",
    "fake":     None,
    "kills":    0
    })

class TestPyMySQL:
    players = dict({})
    def _CreateConnections(self):
        try:
            #self.conn = pymysql.connector.connect(
            self.conn = pymysql.connect(
                user=TestDBConfig['user'],
                password=TestDBConfig['password'],
                host=TestDBConfig['host']) #,
                #buffered=True)
            self.conn.autocommit(True)
            self.cursor = self.conn.cursor()
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def _CreateDatabase(self):
        try:
            db = TestDBConfig['database']
            query = "CREATE DATABASE IF NOT EXISTS " + db 
            self.cursor.execute(query)
            self.conn.select_db(db)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def _CreateTable(self):
        try:
            table = TestDBConfig['table'] 
            query = "CREATE TABLE IF NOT EXISTS " + table + \
                    "   (                                   \
                            steamid VARCHAR(255) NOT NULL,  \
                            PRIMARY KEY (steamid),          \
                            kills INT DEFAULT 0             \
                        )"
            self.cursor.execute(query)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def OnPluginLoad(self):
        try:
            self._CreateConnections(self)
            self._CreateDatabase(self)
            self._CreateTable(self)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def _CloseConnections(self):
        self.cursor.close()
        self.conn.close()
        pass
    def OnPluginUnload():
        self._CloseConnections(self)
        pass
    def _SQLAddPlayer(self, key: str):
        try:
            table = TestDBConfig['table']

            query = str("INSERT INTO " + table + " (steamid, kills) values (\"" + key + "\", 0)")
            self.cursor.execute(query)
            self.conn.commit()
            
            result = self.cursor.rowcount
            return bool(result)
        except Exception as e:
            alog(e)
            return False
        pass
    def _SQLCheckPlayer(self, slot: int, key: str):
        try:
            table = TestDBConfig['table']
            query = str("SELECT kills FROM " + table + " WHERE steamid = '" + key + "' LIMIT 1")
            self.cursor.execute(query)
            result = self.cursor.fetchone()
            if result:
                self.players[slot]['kills'] = int(result[0])
                self.players[slot]['loaded'] = True
            else:
                #no match, no player, so no kills.
                result = self._SQLAddPlayer(self, key)
                self.players[slot]['loaded'] = bool(result)
        except Exception as e:
            self.players[slot]['loaded'] = False
            alog(e)
            alog(traceback.format_exc())
        pass
    def _AddPlayer(self, _slot, _name, _fake, _xuid):
        #without .copy() this would have been making a pointer to the default_player_dict
        #meaning the default_player_dict would be overwritten with the last joining players
        #information.  Pointers, man.
        self.players[_slot] = default_player_dict.copy() 
        try:
            self.players[_slot]['name'] = _name
            self.players[_slot]['fake'] = _fake

            if _fake: 
                self.players[_slot]['steamid'] = _name
            else:
                self.players[_slot]['steamid'] = str(_xuid)

            key = self.players[_slot]['steamid']
            self._SQLCheckPlayer(self, _slot, key)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def OnClientPutInServer(self, 
        _slot: int, #slot on server
        _name: str, #player name
        _type: int, #0=player, 1=bot, 2=???
        _xuid: int):
        try:
            if _type in [0, 1]:
                self._AddPlayer(self, _slot, _name, _type, _xuid)
            else: #type was not 0 or 1!
                alog("_type=" + str(_type))
                pass
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def OnClientConnected(self, 
        _slot: int, #slot on server
        _name: str, #player name
        _xuid: int, #steam id
        _networkID: str, #ip address with port, ex "127.0.0.1:27015"
        _ipAddress: str, #ip address only, ex "127.0.0.1"
        _fake: bool): #client is a bot
        try:
            self._AddPlayer(self, _slot, _name, _fake, _xuid)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def _AddKill(self, slot: int):
        try:
            if self.players[slot]['loaded']:
                table = TestDBConfig['table']
                steamid = self.players[slot]['steamid']

                kills = self.players[slot]['kills'] + 1 
                self.players[slot]['kills'] = kills

                query = str(
                    " UPDATE " + table + \
                    " SET kills = " + str(kills) + \
                    " WHERE steamid = \"" + steamid + "\"")
                self.cursor.execute(query)
                self.conn.commit()
            else:
                key = self.players[slot]['steamid']
                self._SQLCheckPlayer(self, slot, key)
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass
    def OnPlayerDeath(self, event: GameEvent):
        try:
            geks = GameEventKeySymbol_t("attacker")
            aslot = event.GetPlayerSlot(geks).Get()
            if aslot < 0:
                #bomb kill / server / etc
                pass
            else:
                self._AddKill(self, aslot)

                #can get name through here or
                #ap = ADVPlayer(aslot)
                #aname = ap.GetName()
                aname = self.players[aslot]['name']
                akills = self.players[aslot]['kills']

                geks = GameEventKeySymbol_t("userid")
                vslot = event.GetPlayerSlot(geks).Get()
                vname = self.players[vslot]['name']

                alog(str(aname) + " has killed " + str(vname) + " for 1 point and now has " + str(akills) + " point(s).")
        except Exception as e:
            alog(e)
            alog(traceback.format_exc())
        pass


'''
        "player_death":dict({
            "userid":"playercontroller",
            "userid_pawn":"strict_ehandle",
            "attacker":"playercontroller",
            "attacker_pawn":"strict_ehandle",
            "assister":"playercontroller",
            "assister_pawn":"strict_ehandle",
            "assistedflash":"bool",
            "weapon":"string",
            "weapon_itemid":"string",
            "weapon_fauxitemid":"string",
            "weapon_originalowner_xuid":"string",
            "headshot":"bool",
            "dominated":"short",
            "revenge":"short",
            "wipe":"short",
            "penetrated":"short",
            "noreplay":"bool",
            "noscope":"bool",
            "thrusmoke":"bool",
            "attackerblind":"bool",
            "distance":"float",
            "dmg_health":"short",
            "dmg_armor":"byte",
            "hitgroup":"byte",
            "attackerinair":"bool",
            }),
        "other_death":dict({
            "otherid":"short",
            "othertype":"string",
            "attacker":"short",
            "weapon":"string",
            "weapon_itemid":"string",
            "weapon_fauxitemid":"string",
            "weapon_originalowner_xuid":"string",
            "headshot":"bool",
            "penetrated":"short",
            "noscope":"bool",
            "thrusmoke":"bool",
            "attackerblind":"bool",
            }),

'''