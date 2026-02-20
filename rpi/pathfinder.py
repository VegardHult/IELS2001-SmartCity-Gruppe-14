from pathfinding.finder.a_star import AStarFinder
from pathfinding.core.diagonal_movement import DiagonalMovement
from pathfinding.core.grid import Grid
import numpy as np
import copy
import utility
from utility import Car
from utility import Matrix

patrolTime = 20 # Seconds per revolution
radiusConstant = 0.75 # 1 gives circle to outer edge, 0 gives center point

def getPath(grid, start, end):

    grid = Grid(matrix=grid.matrix)

    start = grid.node(start[0], start[1]*(-1)+grid.height-1)
    end = grid.node(end[0], end[1]*(-1)+grid.height-1)

    finder = AStarFinder(diagonal_movement=DiagonalMovement.never)
    path = finder.find_path(start, end, grid)

    coordinates = [[node.x, node.y * (-1) + grid.height-1] for node in path[0]]

    if len(coordinates) == 0:
        return None

    return coordinates

# Get moves for path
def getMoves(grid, path, orientation):
    currentOrientation = orientation
    destinationOrientation = orientation
    moves = []

    start = path[0]
    end = path[-1]
    
    # If destination is reached, or unreachable
    if (start == end):
        return ["I"]
    elif path == None:
        return ["I"]

    # Iterate over path coordinates
    for i, step in enumerate(path):

        # Break if on last path element
        if i == len(path)-1:
            break
        
        # Find orientation for next step
        if path[i+1][0] > step[0]:
            destinationOrientation = 1
        elif path[i+1][0] < step[0]:
            destinationOrientation = 3
        elif path[i+1][1] > step[1]:
            destinationOrientation = 0
        elif path[i+1][1] < step[1]:
            destinationOrientation = 2

        # Get move to hit destination orientation
        while currentOrientation != destinationOrientation:
            difference = destinationOrientation - currentOrientation
            shortestTurn = (difference + 2) % 4 - 2

            # Append orientation move
            if shortestTurn == 2 or shortestTurn == -2:
                moves.append("T")
                currentOrientation = (currentOrientation + 2) % 4
            elif shortestTurn > 0:
                moves.append("R")
                currentOrientation = (currentOrientation + 1) % 4
            elif shortestTurn < 0:
                moves.append("L")
                currentOrientation = (currentOrientation - 1) % 4
        
        # If orientation is correct, appens straigt move
        if currentOrientation == destinationOrientation:
            moves.append("S")
    return moves

# Block grid position of other cars
def blockGrid(grid, cars, currentCar = None):
    blockedGrid = Matrix(grid=grid)
    for car in cars:
        # Don't block current car
        if car == currentCar:
            continue
        # Block other cars positions
        blockedGrid.set(0, car.position[0], car.position[1])
        
        # Block next position
        if len(car.path) >= 2:
            nextPos = car.path[1]
            if utility.onGrid(grid, nextPos[0], nextPos[1]):
                # Block next position of car
                blockedGrid.set(0, nextPos[0], nextPos[1])
    return blockedGrid

# Get new position after move
def getPositionAfterMove(position, orientation, move):
    newPosition = position
    newOrientation = orientation

    # Return if car is idle
    if move == "I":
        return position, orientation
    # Update orientation for tun moves
    elif move == "T":
        newOrientation = (orientation + 2) % 4
    elif move == "R":
        newOrientation = (orientation + 1) % 4
    elif move == "L":
        newOrientation = (orientation - 1) % 4
    # Update position for straight moves
    elif move == "S":
        if orientation == 0:
            newPosition = [position[0], position[1]+1]
        elif orientation == 1:
            newPosition = [position[0]+1, position[1]]
        elif orientation == 2:
            newPosition = [position[0], position[1]-1]
        elif orientation == 3:
            newPosition = [position[0]-1, position[1]]

    return newPosition, newOrientation

def getPatrol(grid, cars, car, elapsedTime):
    # Find center of grid
    center = [(grid.width/2)-0.5,(grid.height/2)-0.5]
    # Circle radius based on smallest grid dimension
    xradius = grid.width / 2 * radiusConstant
    yradius = grid.height / 2 * radiusConstant

    # Individual car time with offset
    carTime = patrolTime / len(cars) * (cars.index(car)+1)

    # Patrol circle angle
    angle = (2 * np.pi) * ((elapsedTime + carTime) % patrolTime) / patrolTime

    # Parametric equations for a circle [1]
    x = int(center[0] + xradius * np.cos(angle))
    y = int(center[1] + yradius * np.sin(angle))
    destination = [x, y]

    return destination

def movesToPosition(moves, start, orientation):
    position = start
    orientation = orientation

    # Keep start position if Idle
    for move in moves:
        if move == "I":
            return start
        
        # Handle turn moves
        if move == "T":
            orientation = (orientation + 2) % 4
        elif move == "R":
            orientation = (orientation + 1) % 4
        elif move == "L":
            orientation = (orientation - 1) % 4

        # Handle straight moves
        elif move == "S":
            if orientation == 0:
                position = [position[0], position[1]+1]
            elif orientation == 1:
                position = [position[0]+1, position[1]]
            elif orientation == 2:
                position = [position[0], position[1]-1]
            elif orientation == 3:
                position = [position[0]-1, position[1]]
    return position

def getCellsAtDistance(x, y, dist):
    cells = []
    # Iterate top and bottom rows
    for iter_x in range(x - dist, x + dist + 1):
        cells.append((iter_x, y - dist))
        cells.append((iter_x, y + dist))
    # Iterate left and right columns (excluding corners already added)
    for iter_y in range(y - dist + 1, y + dist):
        cells.append((x - dist, iter_y))
        cells.append((x + dist, iter_y))
    return cells