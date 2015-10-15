#!/bin/python
#filename: backup.py

import os
import time

source_dir = ["/home/mosquito/Python"]
target_dir = "/home/mosquito/Python/backup"
target = target_dir + time.strftime('%Y%m%d') + '.zip'
back_command = "cp -rf %s %s " % (''.join(source_dir), target)

if os.system(back_command) == 0:
	print "backup success"
else:
	print "backup failed"