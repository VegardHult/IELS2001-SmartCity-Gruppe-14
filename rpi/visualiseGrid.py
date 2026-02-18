import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import MQTT
import json
import time

waitTime = 0.1 # Seconds bewteen updates
settingsKeywords = [
    "size",
    "hospital", 
    "charger"
    ]

# MQTT client settings
ip = "localhost" # MQTT broker ip
port = "1883" # MQTT broker port

# Size of grid, and default values (overwritten by MQTT message)
gridSize = [4,4]

# Topics to subscribe to
subscribeTopics = [
    "program/output"
    "program/run"
    ]

# Settings to wait for before starting program
settings = {}
for setting in settingsKeywords:
    subscribeTopics.append(f"settings/{setting}")
    settings[setting] = None

# Distance to offset arrow from center of node
arrowOffset = 0.2

# Arrow color list
arrowColors = ["r", "c", "m", "y"]

# Update plot function
def update_plot(ax, coords=[], hospital=[0,0], charger=[gridSize[0]-1, gridSize[1]-1]):
    global gridSize

    ax.clear()
    ax.grid()
    ax.set_ylim(-0.5, gridSize[0]+0.5)
    ax.set_xlim(-0.5, gridSize[1]+0.5)

    # Draw hospital and charger
    ax.plot(hospital[0], hospital[1], marker="o", ms=10, color="b")
    ax.plot(charger[0], charger[1], marker="o", ms=10, color="g")

    # Draw cars
    for i, coord in enumerate(coords):
        # Arrow start position
        x = coord[0]
        y = coord[1]
        # Arrow head position
        xtext = coord[0]
        ytext = coord[1]
        # Handle arrow direction
        match coord[2]:
            case 0:
                y += arrowOffset
                ytext -= arrowOffset
            case 1:
                x += arrowOffset
                xtext -= arrowOffset
            case 2:
                y -= arrowOffset
                ytext += arrowOffset
            case 3:
                x -= arrowOffset
                xtext += arrowOffset
        # Draw arrow
        ax.annotate(
            "", # No text
            xy=(x, y), 
            xytext=(xtext, ytext), 
            arrowprops=dict(arrowstyle="->", color=arrowColors[i], lw=3)
        )
    # Draw plot
    plt.draw()
    return

# Main visualiser loop
def main():
    global gridSize
    global subscribeTopics
    global settings

    # Setup MQTT listener
    MQTT.init_client(ip, port)
    
    # Subscribe to topics
    MQTT.init_subscriber(subscribeTopics)

    # ---------------------------------
    # Get setting definitions from MQTT
    # ---------------------------------
    while not (None in settings.values()):
        for setting in settings:
            if MQTT.check_flag(f"settings/{setting}"):
                settings[setting] = json.loads(MQTT.read_message(f"settings/{setting}"))
            
        # Print program output from PI
        if MQTT.check_flag("program/output"):
            print(MQTT.read_message("program/output"))
        # Wait interval
        time.sleep(waitTime)

    gridSize = settings["gridsize"]
    hospital = setting["hospital"]
    charger = settings["charger"]

    # -------------------
    # Plot initialisation
    # -------------------
    fig, ax = plt.subplots()
    plt.ion()
    ax.set_box_aspect(1)
    ax.xaxis.set_major_locator(ticker.MaxNLocator(integer=True))
    ax.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))

    # --------------------
    # Main visualiser loop
    # --------------------
    while True:
        # Stop updating if program is not running
        if MQTT.check_flag("program/run"):
            if MQTT.read_message("program/run") != True:
                # Wait interval
                time.sleep(waitTime) 
                break
        
        # Print program output
        if MQTT.check_flag("program/output"):
            print(MQTT.read_message("program/output"))
        
        # Update plot from car coordinates
        if MQTT.check_flag("visualise/cars"):
            # Get car coordinates
            cars = MQTT.read_message("visualise/cars")
            # Update plot
            update_plot(ax, cars, hospital, charger)
            # Wait interval
            time.sleep(waitTime)

if __name__ == "__main__":
    main()

