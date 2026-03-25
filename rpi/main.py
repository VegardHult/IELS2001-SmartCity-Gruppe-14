import time
import json
import copy
import random

from utility import *
from pathfinderModule import *
from ulykkegenerator import *
from MQTT import *

# ----------------------
# MQTT & Program settings
# ----------------------
ip = "localhost"
port = 1883

waitTime = 0.1  # Seconds between loop iterations

criticalBatteryLimit = 10
batteryLimit = 20
upperChargeLimit = 80

settingsKeywords = ["gridsize", "hospital", "charger", "cars"]

# ----------------------
# Default settings
# ----------------------
defaultSettings = {
    "gridsize": [4, 4],
    "hospital": [0, 0],
    "charger": [3, 3],
    "cars": [[1, 0], [1, 3]]
}

settings = {setting: None for setting in settingsKeywords}

# ----------------------
# Initialize MQTT
# ----------------------
init_client(ip, port)

# Subscribe to initial program/settings topics
subscribeTopics = [
    "program/run", 
    "program/output", 
    "settings/default"
    ]
for topic in subscribeTopics:
    subscribe_topic(topic)

for setting in settingsKeywords:
    subscribe_topic(f"settings/{setting}")

# ----------------------
# Wait for settings
# ----------------------
publish_message(
    "program/output",
    f'Send settings via MQTT:\nsettings/{'\nsettings/'.join(settingsKeywords)} \nOr send "1" to settings/default for default settings.'
)

while None in settings.values():
    # Default settings
    if check_flag("settings/default") and read_message("settings/default") == "1":
        settings = copy.deepcopy(defaultSettings)
        print("[INFO] Default settings applied")

    # Update settings from MQTT messages
    for setting in settings.keys():
        topic = f"settings/{setting}"
        if check_flag(topic):
            settings[setting] = json.loads(read_message(topic))
            print(f"[INFO] Received {setting}: {settings[setting]}")

    time.sleep(waitTime)

print("[INFO] All settings received:", settings)

# ----------------------
# Initialize grid
# ----------------------
width, height = settings["gridsize"]
grid = Matrix(width, height, 1)
blockedGrid = Matrix(grid=grid)
checkGrid = Matrix(grid=grid)
hospital = settings["hospital"]
charger = settings["charger"]

# ----------------------
# Initialize cars
# ----------------------
cars = []
for i, startPos in enumerate(settings["cars"]):
    car = Car(i, startPos, 0, [0, 0])
    car.recieve = 1
    cars.append(car)

    # Subscribe to car topics dynamically
    subscribe_topic(f"car{i}/action")
    subscribe_topic(f"car{i}/battery")

print(f"[INFO] Subscribed to car topics for {len(cars)} cars")

# Assign IDs to cars
IDs = []
for car in cars:
    IDs.append(car.id)

publish_message("program/output", f'Turn on one car at a time, and assign id by sending "id" to "program/assignId \nUnassigned IDs: {IDs}')
while (True):
    if check_flag("program/assignId"):
        id = read_message("program/assignId")
        if id == car.id:
            break
    time.sleep(waitTime)

# ----------------------
# Start program loop
# ----------------------
publish_message("program/output", 'Send "1" to "program/run" to start program.')

# Wait for start signal
while peek_message("program/run") != "1":
    if check_flag("program/output"):
        print(read_message("program/output"))
    time.sleep(waitTime)

print("[INFO] Program started")
publish_message("program/output", "Running program")

# Accident tracking
unassignedAccidents = []
accidents = []
startTime = time.perf_counter()
accidentTime = 0
accidentInterval = 20
accidentVariation = 2

# ----------------------
# Main loop
# ----------------------
while True:
    elapsedTime = time.perf_counter() - startTime

    # -------------------
    # Wait next iteration
    # -------------------
    if peek_message("program/run") != "1":
        time.sleep(waitTime)
        continue

    # ------------------
    # Generate accidents
    # ------------------
    randomInterval = accidentVariation * (random.random() - 0.5)
    if elapsedTime - accidentTime >= accidentInterval + randomInterval:
        accident = ulykkegenerator(width, height)
        if not(accident == hospital or accident == charger):
            unassignedAccidents.append(accident)
            accidents.append(accident)
            accidentTime = elapsedTime
            randomInterval = accidentVariation * (random.random() - 0.5)

    # -----------
    # Update cars
    # -----------
    for car in cars:
        # -----------------------
        # Process action messages
        # -----------------------
        actionTopic = f"car{car.id}/action"
        if check_flag(actionTopic):
            move = read_message(actionTopic)

            # Validate move
            if car.moves and move != car.moves[0]:
                print(f"[ERROR] Car {car.id}: received {move}, expected {car.moves[0]}")

            # Update car position
            blockedGrid = blockGrid(grid, cars, car)
            car.position, car.orientation = getPositionAfterMove(car.position, car.orientation, move)
            car.recieve = 1

        # ---------------
        # Battery updates
        # ---------------
        batteryTopic = f"car{car.id}/battery"
        if check_flag(batteryTopic):
            car.battery = int(read_message(batteryTopic))

        # ------------------------
        # Battery & charging logic
        # ------------------------
        if car.battery < batteryLimit and car.busy == 0:
            car.destination = charger
            car.state = "Default"

        if car.battery < criticalBatteryLimit and car.busy == 0:
            car.destination = charger
            car.busy = 1
            car.state = "Default"

        if car.position == charger and car.position == car.destination:
            car.state = "Charging"

        if car.state == "Charging" and car.battery > upperChargeLimit:
            car.busy = 0

        if car.state == "Charging" and car.battery == 100:
            car.state = "Default"

        # ------------------
        # Handle emergencies
        # ------------------
        if car.position in accidents and car.position == car.destination:
            print("reached accident")
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
        if not car.busy and car.state == "Default":
            car.destination = getPatrol(grid, cars, car, elapsedTime)

    # -------------------------------
    # Assign accidents to nearest car
    # -------------------------------
    for accident in unassignedAccidents:
        nearest_id = naresteBil(grid, cars, accident)
        if nearest_id is not None:
            cars[nearest_id].destination = accident
            cars[nearest_id].busy = 1
            cars[nearest_id].state = "Emergency"
            unassignedAccidents.remove(accident)

    # ---------------------
    # Compute paths & moves
    # ---------------------
    for car in cars:
        blockedGrid = blockGrid(grid, cars, car)
        path = getPath(blockedGrid, car.position, car.destination)

        # If no path, find closest reachable cell
        distance = 1
        shortestPath = width * height
        while path is None:
            cells = pathfinder.getCellsAtDistance(car.destination[0], car.destination[1], distance)
            for cell in cells:
                if not utility.onGrid(blockedGrid, cell[0], cell[1]):
                    continue
                alt_path = pathfinder.getPath(blockedGrid, car.position, cell)
                if alt_path and len(alt_path) < shortestPath:
                    path = alt_path
                    shortestPath = len(alt_path)
            distance += 1

        car.path = path
        car.moves = pathfinder.getMoves(blockedGrid, car.path, car.orientation)

    # -----------------------------
    # Publish moves & state to cars
    # -----------------------------
    for car in cars:
        if car.recieve and car.moves:
            publish_message(f"car{car.id}/nextaction", car.moves[0])
            publish_message(f"car{car.id}/mode", car.state)
            car.recieve = 0

    # ---------
    # Visualize
    # ---------
    carspos = []
    for car in cars:
        carspos.append([car.position[0], car.position[1], car.orientation, car.path])

    publish_message("visualize/cars", carspos)
    publish_message("visualize/accidents", accidents)

    # --------------
    # Program output
    # --------------
    if check_flag("program/output"):
        print(read_message("program/output"))

    time.sleep(waitTime)