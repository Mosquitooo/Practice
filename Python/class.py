#!/usr/bin/python
# filename: class.py

class Person:
	def __init__(self,name):
		self.name = name
		print 'Person init'
	def SayHi(self):
		''' really '''
		print 'hello, how are you, i am %s' % self.name
	def __del__(self):
		print 'Person delete'

class Student(Person):
	def __init__(self, name):
		Person.__init__(self,name)
		print 'student init'
	def SayHi(self):
		print 'i am a student, my name is %s' % self.name
	def __del__(self):
		print 'student delete'

p = Person('lilei')
p.SayHi()
s = Student('hanmeimei')
s.SayHi()