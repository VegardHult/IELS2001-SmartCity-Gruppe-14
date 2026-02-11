import copy

# Car class
class Car:
    "id" = 0
    "position" = []
    "orientation" = 0
    "destination" = []
    "path" = []
    "moves" = []
    "battery" = 100
    "state" = "Default"
    "busy" = 0

def safeCopy(var1, var2=None, var3=None, var4=None):
    out1 = copy.deepcopy(var1)
    out2 = copy.deepcopy(var2)
    out3 = copy.deepcopy(var3)
    out4 = copy.deepcopy(var4)
    return out1, out2, out3, out4