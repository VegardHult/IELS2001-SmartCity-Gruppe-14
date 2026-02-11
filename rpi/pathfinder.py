from pathfinding.finder.a_star import AStarFinder
from pathfinding.core.diagonal_movement import DiagonalMovement
from pathfinding.core.grid import Grid
import numpy as np
import utility
from utility import Car

patrolTime = 5 # Seconds per revolution
relativeRadius = 0.75 # Patrol radius based on smallest grid dimension

def findPath(grid, start, end):

    grid = Grid(matrix=grid)

    start = grid.node(start[0], start[1])
    end = grid.node(end[0], end[1])

    finder = AStarFinder(diagonal_movement=DiagonalMovement.never)
    path = finder.find_path(start, end, grid)
    print(f"path {path}")

    coordinates = [[node.x, node.y] for node in path]
    return coordinates

    # Can also be visualized using the library's tools or printed
    print(grid.grid_str(path=path))

# Get moves for path
def getMoves(grid, start, end, orientation):
    path = findPath(grid, start, end)
    currentOrientation = orientation
    destinationOrientation = orientation
    moves = []
    
    # If destination is reached, or unreachable
    if (start == end) or (not path):
        moves.append(["I"])
        return moves

    # Iterate over path coordinates
    for i, step in enumerate(path):

        # Break if list contains one element
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
def blockGrid(grid, cars, currentCar):
    for car in cars:
        blockedGrid = utility.safeCopy(grid)
        # Don't block current car
        if car == currentCar:
            continue
        # Block current car positions
        blockedGrid[car.path[0][0], car.path[0][1]] = 0
        
        # Block next position
        if len(car.path) > 2:
            # Block next car position
            blockGrid[car.path[1][0]][car.path[1][1]] = 0
    return blockedGrid

# Update moves list in all cars
def updateAllMoves(grid, cars):
    moves = []
    blockedGrid = grid
    for car in cars:
        blockedGrid = blockGrid(grid, cars, car)
        car.moves = getMoves(grid, car.position, car.destination, car.orientation)
    return cars

# Get new position after move
def getPosition(position, orientation, move):
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
    # Size of grid
    gridSize = [len(grid[0]),len(grid)]

    # Find center of grid
    center = [gridSize[0]/2,gridSize[1]/2]
    # Circle radius based on smallest grid dimension
    radius = min(gridSize)*0.75

    # Individual car time with offset
    carTime = patrolTime / len(cars) * (car.id+1)
    # Patrol circle angle
    angle = (2 * np.pi) * ((elapsedTime + carTime) % patrolTime) / patrolTime
    
    # Parametric equations for a circle [1]
    x = int(center[0] + radius * np.cos(angle))
    y = int(center[1] + radius * np.sin(angle))
    destination = [x, y]

    return destination