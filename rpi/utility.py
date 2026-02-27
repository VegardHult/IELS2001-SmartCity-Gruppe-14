import copy
import json

# Car class
class Car:
    def __init__(self, id, pos, ori = 0, dest = None, path = None, mov = "I", batt = 100, state = "Default", busy = 0, rec = 1):
        self.id = id
        self.position = pos
        self.orientation = ori
        if not dest:
            self.destination = pos
        else:
            self.destination = dest
        if not path:
            self.path = [pos]
        else:
            self.path = path
        self.moves = mov
        self.battery = batt
        self.state = state
        self.busy = busy
        self.recieve = rec

class Matrix:
    def __init__(self, width=0, height=0, value=0, grid=None):
        if type(grid) == Matrix:
            self.matrix = copy.deepcopy(grid.matrix)
            self.width = copy.copy(grid.width)
            self.height = copy.copy(grid.height)
        elif grid != None:
            self.matrix = copy.deepcopy(grid)
            self.width = len(grid[0])
            self.height = len(grid)
        else:
            self.matrix = [[value for _ in range(width)] for _ in range(height)]
            self.width = width
            self.height = height
        self.size = [width, height]

    def get(self, x, y):
        y = y * (-1) + (self.height-1)
        return self.matrix[y][x]
    
    def set(self, value, x=None, y=None):
        if x == None and y == None:
            for y in range (self.height-1):
                y = (y * (-1)) + (self.height-1)
                for x in range (self.width-1):
                    self.set(value, x, y)
        elif x == None:
            y = (y * (-1)) + (self.height-1)
            for x in range (self.width-1):
                self.set(value, x, y)
        elif y == None:
            for y in range (self.height):
                y = (y * (-1)) + (self.height-1)
                self.set(value, x, y)
        else:
            y = (y * (-1)) + (self.height-1)
            self.matrix[y][x] = value

def onGrid(grid, x, y):
    if (x > (grid.width-1)) or (x < 0) or (y > (grid.height-1)) or (y < 0):
        return False
    return True

def printGrid(grid, car):
    pgrid = Matrix(grid=grid)

    if car.orientation == 0:
        text = "^"
    elif car.orientation == 1:
        text = ">"
    elif car.orientation == 2:
        text = "v"
    else:
        text = "<"

    for y, row in enumerate(pgrid.matrix):
        for x, col in enumerate(row):
            val = pgrid.get(x, y)
            pgrid.set(str(val), x, y)

    pgrid.set(text, car.position[0], car.position[1])


    print("------------------------")
    for row in pgrid.matrix:
        print(row)