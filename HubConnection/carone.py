__author__ = 'Chris Lynch'

from connection import *


class CarOne(object):
    def place(self, data):
        firebase.put('/score', 'Car 1', {'data': data})
        print(data)
