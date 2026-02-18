from utility import Car
from utility import Matrix
import utility
import pathfinder
import MQTT
import time
import random
import json
import copy
import ulykkegenerator


# MQTT client settings
ip = "localhost" # MQTT broker ip
port = 1883 # MQTT broker port

# Settings to edit by MQTT
settingsKeywords = [
    "gridsize",
    "cars"
]

# Time interval for time.sleep between refreshes
waitTime = 0.1 # Seconds

# Battery settings
criticalBatteryLimit = 10 # %
batteryLimit = 20 # %
upperChargeLimit = 80 # %

# Times
startTime = time.perf_counter() # Program start time
accidentTime = startTime # Time of last generated accident
accidentInterval = 10 # Accident generation interval in seconds
accidentVariation = 2 # Size of random number interval variation
randomInterval = accidentVariation * (random.random()-0.5)

# Setup grid
width = 4
height = 4
grid = Matrix(width, height, 1) # Generate grid
checkGrid = Matrix(grid=grid)
blockedGrid = Matrix(grid=grid)
hospital = [0,0] # Hospital coordinates, lower left corner
charger = [width-1, height-1] # Charger coordinates, upper rigth corner

# Startposition of cars, used to create cars
startPositions = [[0,3],[3,3]]
# List of accident coordinates
unassignedAccidents = []
accidents = []

# Setup MQTT listener
MQTT.init_client(ip, port)
# Topics to subscribe to
subscribeTopics = [
    "program/run",
    "program/output"
    ]

# Setting handling
settings = {}
for setting in settingsKeywords:
    subscribeTopics.append(f"settings/{setting}")
    settings[setting] = None

# --------------------------------
# Set variables from MQTT settings
# --------------------------------

"""
MQTT.publish_message("program/output", f"Open visualiser first, then send settings by MQTT:\nsettings/{"\nsettings/".join(settingsKeywords)}")
while (None in settings.values()):
    if MQTT.check_flag("program/output"):
        print(f"{MQTT.read_message("program/output")}")
    #break # Comment out to use MQTT start function
    for setting in settings.keys():
        if MQTT.check_flag(f"settings/{setting}"):
            settings[setting] = json.loads(MQTT.read_message(f"settings/{setting}"))
    # Wait interval
    time.sleep(waitTime)
"""

# Creating car objects
cars = []
for i, startPosition in enumerate(startPositions):
    cars.append(Car(i, startPosition, 0, [1,0]))
    # Subscribe to every car topic
    subscribeTopics.append(f"car{i}/action")
    subscribeTopics.append(f"car{i}/battery")
    MQTT.subscribe_topic(f"car{i}/action")
    MQTT.subscribe_topic(f"car{i}/battery")

# Subscribe to topics
MQTT.init_subscriber(subscribeTopics)

# ---------------------
# Wait for start signal
# ---------------------
MQTT.publish_message("program/output", f'Send "1" to "program/run" to start program.')
while True:
    # Print program output
    if MQTT.check_flag("program/output"):
        print(f"{MQTT.read_message("program/output")}")
    # Check for start signal
    if MQTT.check_flag("program/run"):
        if MQTT.read_message("program/run") == "1":
            break
            

# ---------
# Main loop
# ---------
MQTT.publish_message("program/output", "Running program")
while True:
    # Print program output
    if MQTT.check_flag("program/output"):
        print(f"{MQTT.read_message("program/output")}")
    
    # Stop program on MQTT message
    if MQTT.check_flag("program/run"):
        if MQTT.read_message("program/run") == "0":
            MQTT.publish_message("program/output", " program")
            time.sleep(waitTime)
            break
        else:
            MQTT.publish_message("program/output", "Running program")
    time.sleep(waitTime)
    
    # Get elapsed time
    elapsedTime = time.perf_counter() - startTime

    # ------------------
    # Generate accidents
    # ------------------
    if accidentTime > elapsedTime + accidentInterval + (randomInterval*accidentVariation):
        accident = ulykkegenerator.ulykkegenerator(width, height)
        unassignedAccidents.append(accident)
        accidents.append(accident)
        accidentTime = elapsedTime
        randomInterval = accidentVariation * (random.random()-0.5)

    # -------------------
    # Update car position
    # -------------------
    for car in cars:
        # Check if car action updated
        carTopic = f"car{car.id}/action"
        if MQTT.check_flag(carTopic):
            # Read completed move
            move = MQTT.read_message(carTopic)
            # print(f"at recieve: car {car.id}: {car.moves}")
            # Print error message if car made wrong move
            if move != car.moves[0]:
                print(f"Error car {car.id}: Recieved {move}, expected {car.moves[0]}")
            blockedGrid = pathfinder.blockGrid(grid, cars, car)
            if blockedGrid.get(car.position[0], car.position[1]) == 0:
                print(f"Error car {car.id}: Moved to blocked node")
            # Update car position and orientation after move
            car.position, car.orientation = pathfinder.getPositionAfterMove(car.position, car.orientation, move)
            # Set car ready to recieve next move
            car.recieve = 1

        # ----------------
        # Battery Handling
        # ----------------
        # Check for battery update
        if MQTT.check_flag(f"car{car.id}/battery"):
            # Get battery level
            car.battery = MQTT.read_message(f"car{car.id}/battery")

        # Send to charger if low battery and not busy
        if car.battery < batteryLimit and car.busy == 0:
            car.destination = charger
            car.state = "Default"

        # Send to charger if critical battery and not busy, set busy
        if car.battery < criticalBatteryLimit and car.busy == 0:
            car.destination = charger
            car.busy = 1
            car.state = "Default"

        # Start charging when on charger
        if car.position == charger and car.position == car.destination:
            car.state = "Charging"

        # Car not busy when reached 80% charge
        if car.state == "Charging" and car.battery > upperChargeLimit:
            car.busy = 0

        # Set default state if fully charged
        if car.state == "Charging" and car.battery == 100:
            car.state = "Default"

        # -------------------
        # Reached destination
        # -------------------
        if car.position in accidents and car.position == car.destination:
            accidents.remove(car.position)
            car.destination = hospital
            car.busy = 1
            car.state = "Emergency"
        elif car.position == hospital and car.position == car.destination:
            car.busy = 0
            car.state = "Default"
        # -------------
        # Handle patrol
        # -------------
        if (car.busy == False) and (car.state == "Default"):
            car.destination = pathfinder.getPatrol(grid, cars, car, elapsedTime)
    # xxxxxxxxxxxxxxxxx
    # Main car loop end
    # xxxxxxxxxxxxxxxxx

    # ----------------------
    # Assign accident to car
    # ----------------------
    for accident in unassignedAccidents:
        id = ulykkegenerator.naresteBil(grid, cars, accident)
        if id is not None:
            cars[id].destination = accident
            cars[id].busy = 1
            cars.state = "Emergency"
            unassignedAccidents.remove(accident)

    # -----------------
    # Update moves list
    # -----------------
    for car in cars:
        # Get blocked grid
        blockedGrid = pathfinder.blockGrid(grid, cars, car)
        # Get car path
        path = pathfinder.getPath(blockedGrid, car.position, car.destination) 
        
        # If destination is blocked, find closest cell to target
        distance = 1
        shortestPath = width * height
        # Run until there is a valid path
        while path == None:
            # Find cells at distance from poit
            cells = pathfinder.getCellsAtDistance(car.destination[0], car.destination[1], distance)
            alt_paths = []
            # Iterate over cells
            for cell in cells:
                # Continue to next cell if not on grid
                if not utility.onGrid(blockedGrid, cell[0], cell[1]):
                    continue
                # Test alternate path
                alt_path = pathfinder.getPath(blockedGrid, car.position, cell)
                if alt_path != None:
                    # Choose shortest path at shortest distance from destination
                    if len(alt_path) < shortestPath:
                        alt_paths.append(alt_path)
                        shortestPath = len(alt_path)
                        path = alt_path
            distance += 1

        # Write path to car
        car.path = path
        # Get moves
        moves = pathfinder.getMoves(blockedGrid, car.path, car.orientation)
        # Write moves to car
        car.moves = moves

    # -----------------
    # Send MQTT to cars
    # -----------------
    for car in cars:
        # Check if car is ready to recieve move
        if car.recieve:
            # print(f"at send: car {car.id}: {car.moves}")
            # blockedGrid = pathfinder.blockGrid(grid, cars, car)
            # utility.printGrid(blockedGrid, car)
            # Send next move
            MQTT.publish_message(f"car{car.id}/nextaction", car.moves[0])
            # Send car state
            MQTT.publish_message(f"car{car.id}/state", car.state)
            # Set not ready to recieve
            car.recieve = 0

