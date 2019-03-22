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
          './nixplot --debug --PNG --out=./tst/output.png --width=1210 --height=300 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77 --scene=00:01:00/0.2,00:05:00/0.8',
          #'./nixplot --debug --PDF --out=./tst/output.pdf --width=1210 --height=300 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77 --scene=00:01:00/0.2,00:05:00/0.8',
          #'./nixplot --PNG --out=./tst/output.png --width=1210 --height=300 --xtype=T --ytype=N --pen0=00:00:00/0.11,00:01:00/0.4,00:02:00/0.25,00:03:00/0.3,00:04:00/0.77 --scene=00:01:00/0.2,00:05:00/0.8',
          #'./nixplot --debug --out=./tst/output.png --xtype=T --ytype=N --width=640 --height=480',
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
