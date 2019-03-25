#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import os.path
import shutil
import time


def do_tst(cmd):
    print('Start command: <%s>' % cmd)
    start_time = time.time()
    cmd_list = cmd.split(' ')
    os.spawnv(os.P_WAIT, cmd_list[0], cmd_list)
    print('Stop command: <%s> Time = %d' % (cmd, time.time() - start_time))


commands=(
          #'./nixplot --debug --PNG --out=./tst/output.png --xtype=T --ytype=N --pen0=00:00:00/50,00:01:00/34,00:02:00/73,00:03:00/62,00:04:00/21',
          #'./nixplot --debug --PNG --out=./tst/output.png --xtype=T --ytype=N',
          #'./nixplot --debug --PNG --out=./tst/output.png --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77',
          #'./nixplot --debug --PNG --out=./tst/output.png --width=1280 --height=960 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77',
          #'./nixplot --debug --PNG --out=./tst/output.png --width=1280 --height=480 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77',
          #'./nixplot --debug --PNG --out=./tst/output.png --width=1210 --height=300 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77',
          #'./nixplot --debug --PNG --out=./tst/output.png --width=1210 --height=300 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77 --scene=00:01:00/0.2,00:05:00/0.8',
          #'./nixplot --debug --PDF --out=./tst/output.pdf --width=1210 --height=300 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77 --scene=00:01:00/0.2,00:05:00/0.8',
          #'./nixplot --PNG --out=./tst/output.png --width=1210 --height=300 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77 --scene=00:01:00/0.2,00:05:00/0.8',
          #'./nixplot --debug --out=./tst/output.png --xtype=T --ytype=N --width=640 --height=480',
          #'./nixplot --debug --PNG --out=./tst/output.png --xtype=T --ytype=N --width=1321 --height=522',
          # './nixplot --PDF --out=./tst/output.pdf --xtype=T --ytype=N --width=1321 --height=522',
          #'./nixplot --PNG --out=./tst/output.png --xtype=T --ytype=N --width=1321 --height=533 --scene=01:00:00/185.55,23:59:59/192.21 --pen0=01:00:00/190.24,01:00:00/191.02,02:00:00/189.15,02:00:00/190.75,03:00:00/189.86,03:00:00/191.11,04:00:00/192.15,04:00:00/192.21,05:00:00/191.75,05:00:00/192.11,06:00:00/185.55,06:00:00/186.71,07:00:00/188.37,07:00:00/188.68,08:00:00/191.66,08:00:00/191.88,09:00:00/189.23,09:00:00/190.03,10:00:00/187.63,10:00:00/189.41,11:00:00/190.25,11:00:00/190.63,12:00:00/191.86,12:00:00/192.06,13:00:00/191.05,13:00:00/191.21,14:00:00/188.84,14:00:00/189.42,15:00:00/189.47,15:00:00/190.51,16:00:00/191.86,16:00:00/191.88,17:00:00/191.53,17:00:00/191.8,18:00:00/191.37,18:00:00/191.7,19:00:00/191.15,19:00:00/191.61,20:00:00/189.14,20:00:00/190.64,21:00:00/190.25,21:00:00/191.35,22:00:00/192.05,22:00:00/192.18,23:00:00/191.17,23:00:00/191.52,23:59:59/190.5,23:59:59/191.06',
          './nixplot --PDF --out=./tst/output.pdf --xtype=T --ytype=N --width=1321 --height=533 --scene=01:00:00/185.55,23:59:59/192.21 --pen0=01:00:00/190.24,01:00:00/191.02,02:00:00/189.15,02:00:00/190.75,03:00:00/189.86,03:00:00/191.11,04:00:00/192.15,04:00:00/192.21,05:00:00/191.75,05:00:00/192.11,06:00:00/185.55,06:00:00/186.71,07:00:00/188.37,07:00:00/188.68,08:00:00/191.66,08:00:00/191.88,09:00:00/189.23,09:00:00/190.03,10:00:00/187.63,10:00:00/189.41,11:00:00/190.25,11:00:00/190.63,12:00:00/191.86,12:00:00/192.06,13:00:00/191.05,13:00:00/191.21,14:00:00/188.84,14:00:00/189.42,15:00:00/189.47,15:00:00/190.51,16:00:00/191.86,16:00:00/191.88,17:00:00/191.53,17:00:00/191.8,18:00:00/191.37,18:00:00/191.7,19:00:00/191.15,19:00:00/191.61,20:00:00/189.14,20:00:00/190.64,21:00:00/190.25,21:00:00/191.35,22:00:00/192.05,22:00:00/192.18,23:00:00/191.17,23:00:00/191.52,23:59:59/190.5,23:59:59/191.06',
          )


def test():
    print('NIXPLOT test start .. ok')    

    for i, command in enumerate(commands):
        if os.path.exists('./tst/output.png'):
            os.remove('./tst/output.png')
        if os.path.exists('./tst/output.pdf'):
            os.remove('./tst/output.pdf')
            
        do_tst(command)
        
        if os.path.exists('./tst/output.png'):
            if len(commands) == 1:
                os.system('eog ./tst/output.png&')
            else:
                os.system('eog ./tst/output.png')
        elif os.path.exists('./tst/output.pdf'):
            if len(commands) == 1:
                os.system('evince ./tst/output.pdf&')
            else:
                os.system('evince ./tst/output.pdf')
        else:
            print('Output file not found')        
        
    print('NIXPLOT test stop .. ok')

    
if __name__=='__main__':
    test()
