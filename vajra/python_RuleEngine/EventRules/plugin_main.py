import EventRules.rules as Rules
from DB.db_evn import get_Rules
import time
import threading

win_DataBase_rules=[]
linux_DataBase_rules=[]
win_RulesDict={
        # Rules.apt_get_spawnshell:['4', 'technique', 'T1011', 'http://dgyauwdgbawdg'],
        # Rules.ash_spawn_shell:['4', 'technique', 'T1001', 'http://dgyauwdgbawdg']
    }
linux_RulesDict={
        # Rules.apt_get_spawnshell:['4', 'technique', 'T1011', 'http://dgyauwdgbawdg'],
        # Rules.ash_spawn_shell:['4', 'technique', 'T1001', 'http://dgyauwdgbawdg']
    }

def UpdateRules():
    while True:
        global win_DataBase_rules, linux_DataBase_rules
        win_DataBase_rules=get_Rules('Windows')
        linux_DataBase_rules=get_Rules('Linux')
        time.sleep(2)

x=threading.Thread(target=UpdateRules,daemon=True)
x.start()
time.sleep(2)

def ProcessEventProcessor(os_name):
    if (os_name == 'Linux'):
        for linux_DataBase_rule in linux_DataBase_rules:
            linux_RulesDict[linux_DataBase_rule[1]]=[linux_DataBase_rule[3],linux_DataBase_rule[4],linux_DataBase_rule[5],linux_DataBase_rule[6],linux_DataBase_rule[2],linux_DataBase_rule[7],linux_DataBase_rule[8]]
        # exec(DataBase_rule[2])linux_DataBase_rules
        # for DataBase_rule in DataBase_rules:
        #     RulesDict[DataBase_rule[1]]=[DataBase_rule[3],DataBase_rule[4],DataBase_rule[5],DataBase_rule[6]]
        return linux_RulesDict
    else:
        for win_DataBase_rule in win_DataBase_rules:
            win_RulesDict[win_DataBase_rule[1]]=[win_DataBase_rule[3],win_DataBase_rule[4],win_DataBase_rule[5],win_DataBase_rule[6],win_DataBase_rule[2],win_DataBase_rule[7],win_DataBase_rule[8]]
        # exec(DataBase_rule[2])linux_DataBase_rules
        # for DataBase_rule in DataBase_rules:
        #     Rule
        return win_RulesDict
