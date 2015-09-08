__author__ = 'Chris Lynch'

from connection import *


class CarOne(object):
    def place(self, data):
        firebase.put('/score', 'Car 1', {'data': data})
        print(data)

    def get(self):
        result = firebase.get('/score', 'Car 1')
        return result