from utility import Car
import pathfinder
import MQTT
import time
import random
import json
import ulykkegenerator

# Settings to edit by MQTT
settingsKeywords = [
    "gridsize",
    "cars"
]

waitTime = 0.1

# MQTT client settings
ip = "192.168.137.74" # MQTT broker ip
port = "1883" # MQTT broker port

# Battery settings
criticalBatteryLimit = 20
batteryLimit = 40
upperChargeLimit = 80

# Times
startTime = time.perf_counter() # Program start time
accidentTime = startTime # Time of last generated accident
accidentInterval = 10 # Accident generation interval in seconds
accidentVariation = 2 # Size of random number interval variation

# Setup grid
gridSize = [4, 4] # Grid dimensions
grid = [[1 for _ in range(gridSize[0])] for _ in range(gridSize[1])] # Generate grid
hospital = [0,0] # Hospital coordinates
charger = [gridSize[0]-1, gridSize[1]-1]

# Startposition of cars, used to create cars
startPositions = [[0,0], [3,3]]

# List of car objects
cars = {}
# List of accident coordinates
accidents = []

# Setup MQTT listener
MQTT.init_client(ip, port)
# Topics to subscribe to
subscribeTopics = [
    "program/run"
    ]

# Setting handling
settings = {}
for setting in settingsKeywords:
    subscribeTopics.append(f"settings/{setting}")
    settings[setting] = None

# Creating car objects
for i, startPosition in enumerate(startPositions):
    cars[i] = Car(i, startPosition, 0, startPosition)
    # Subscribe to every car topic
    subscribeTopics.append(f"car{i}/action")
    subscribeTopics.append(f"car{i}/battery")

# Subscribe to topics
MQTT.init_subscriber(subscribeTopics)

# -------------------------
# Wait for settings by MQTT
# -------------------------
MQTT.publish_message("program/output", f"Open visualiser first, then send settings by MQTT:\n{"\n".join(subscribeTopics)}")
while not (None in settings.values()):
    #break # Comment out to use MQTT start function
    for setting in settings:
        if MQTT.check_flag(f"visualiser/{setting}"):
            setting = json.loads(MQTT.read_message(f"visualiser/{setting}"))
    # Wait interval
    time.sleep(waitTime)

# --------------------------------
# Set variables from MQTT settings
# --------------------------------
# Grid dimensions
gridSize = settings["gridsize"]

# Initiate Car objects from startPositions
startPositions = json.loads(settings["cars"])
for i, pos in startPositions: 
    cars[i] = Car(i, pos)

# ---------
# Main loop
# ---------
MQTT.publish_message("program/output", f'Send "1" to "program/run" to start program.')
while True:
    # Stop program on MQTT message
    if MQTT.check_flag("program/run"):
        if MQTT.read_message("program/run") == False:
            time.sleep(waitTime)
            continue
    
    # Get elapsed time
    elapsedTime = time.perf_counter() - startTime

    # ------------------
    # Generate accidents
    # ------------------
    if accidentTime > elapsedTime + accidentInterval + ((random.random()-0.5)*accidentVariation):
        accidents.append(ulykkegenerator.ulykkegenerator(gridSize[0]-1, gridSize[1]-1))
        accidentTime = elapsedTime
    
    # -------------
    # Car loop
    # -------------
    for car in cars:
        # Check if car action updated
        carTopic = f"car{car.id}/action"
        if MQTT.check_flag(carTopic):
            # Send next move
            if len(car.moves) > 1:
                MQTT.publish_message(f"car{car.id}/nextaction", car.moves[1])
            else:
                MQTT.publish_message(f"car{car.id}/nextaction", car.moves[0])

            # Read completed move
            move = MQTT.read_message(carTopic)
            # Print error message if car made wrong move
            if MQTT.read_message(carTopic) != car.moves[0]:
                print("Error: Wrong move made")
            # Update car position and orientation after move
            car.position, car.orientation = pathfinder.getPosition(car.position, car.orientation, move)


        # ----------------
        # Battery Handling
        # ----------------
        # Check for battery update
        if MQTT.check_flag(f"car{car.id}/battery"):
            # Get battery level
            car.battery = MQTT.read_message(f"car{car.id}/battery")

        # Send to charger if critical battery level
        if car.battery < criticalBatteryLimit and car.busy == 0:
            car.destination = charger
            car.busy = 1
            car.state = "Default"

        # Start charging when on charger
        if car.position == charger:
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
        if car.position in accidents:
            accidents.remove(car.position)
            car.destination = hospital
            car.busy = 1
            car.state = "Emergency"
        elif car.position == car.destination:
            car.busy = 0
            car.state = "Default"

        # Handle patrol
        if (car.busy == False) and (car.state == "Default"):
            car.destination = pathfinder.getPatrol(elapsedTime)
    # xxxxxxxxxxxxxxxxx
    # Main car loop end
    # xxxxxxxxxxxxxxxxx

    # ----------------------
    # Assign accident to car
    # ----------------------
    for accident in accidents:
        id = ulykkegenerator.naresteBil(grid, cars, accident)
        cars[id].destination = accident
        cars[id].busy = 1
        cars.state = "Emergency"


    # -----------------
    # Update moves list
    # -----------------
    pathfinder.updateAllMoves(grid, cars)

    # -----------------
    # Send MQTT to cars
    # -----------------
    for car in cars:
        # Send next move
        if len(car.moves) > 1:
            MQTT.publish_message(f"car{car.id}/nextaction", car.moves[1])
        else:
            MQTT.publish_message(f"car{car.id}/nextaction", car.moves[0])
        # Send car state
        MQTT.publish_message(f"car{car.id}/state", car.state)

