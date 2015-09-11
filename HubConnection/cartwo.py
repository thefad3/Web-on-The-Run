__author__ = 'Chris Lynch'

from connection import *


class CarTwo(object):
    def place(self, data):
        firebase.put('/score', 'Car 2', {'data': data})
        print(data)

    def get(self):
        result = firebase.get('/score', 'Car 2')
        return result