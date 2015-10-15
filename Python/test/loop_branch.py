
#!/bin/python

number = 23
runing = True

def success():
	print "loop over"

while runing:
	guess = int(raw_input('enter a number'))
	if guess == number:
		print "you get it"
		runing = False;
		#break
	elif guess < number:
		print "<"
	else:
		print ">"
else:
	success()
print "done"
print success.__doc__