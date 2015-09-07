__author__ = 'Chris Lynch'
#!/usr/bin/env python

import json
import sys
import atexit
from firebase import firebase

_firebase_name = 'mariocarts'
firebase = firebase.FirebaseApplication('https://' + _firebase_name + '.firebaseio.com/', None)

test = firebase.put('/score', 'Car 1', {'testJson': 'Show me the Data'})

if __name__ == '__main__':
    def __init__():
        print('Firebase Test Complete.')