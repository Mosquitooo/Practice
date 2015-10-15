#!/bin/python
#filename: backup.py

import os
import time

source_dir = ["/home/mosquito/Python/test"]
target_dir = "/home/mosquito/Python/backup/"
today = time.strftime('%Y%m%d')
back_command = "cp -rf %s '%s' " % (''.join(source_dir), target)

if os.system(back_command) == 0:
	print "backup success"
else:
	print "backup failed"