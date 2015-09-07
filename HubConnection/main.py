__author__ = 'Chris Lynch'
#!/usr/bin/env python

import json
import sys
import atexit
from firebase import firebase

if __name__ == '__main__':
    freeze_support()

print 'Hello World! Im a Python.'

firebase_name = 'mariocarts'


firebase = firebase.FirebaseApplication('https://'+firebase_name+'.firebaseio.com/', None)

test = firebase.put('/score', 'Car 1', {
    'testJson':'Show me the Data'
})

print test

exit()
