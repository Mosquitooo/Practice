#!/usr/bin/python

import urllib
import re

ADDR = "http://www.heibanke.com/lesson/crawler_ex01/"

def gethtml(url):
	page = urllib.urlopen(url)
	html = page.read()
	return html


html = gethtml(ADDR)
print html
