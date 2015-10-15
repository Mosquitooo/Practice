#!/usr/bin/python
#using file input and out put

poem = ''' this is fun
		if you wanna make your work also fun
		use python'''

f = file('poem.txt','w')
f.write(poem);
f.close()

f = file('poem.txt','r')
while True:
	line = f.readline()
	if len(line) == 0:
		break
	print line

f.close()