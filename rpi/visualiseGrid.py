import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import MQTT
import json
import time
import copy

waitTime = 0.1 # Seconds bewteen updates
settingsKeywords = [
    "gridsize",
    "hospital",
    "charger"
    ]

# MQTT client settings
ip = "localhost" # MQTT broker ip
port = 1883 # MQTT broker port

# Default settings
defaultSettings = {
    "gridsize": [4, 4],
    "hospital": [0, 0],
    "charger": [3, 3],
    "cars": [[1, 0], [1, 3]]
}
# Initiate grid size varable
gridSize = defaultSettings["gridsize"]

# Topics to subscribe to
subscribeTopics = [
    "program/output",
    "program/run",
    "settings/default",
    "visualize/cars",
    "visualize/paths",
    "visualize/accidents"
    ]

# Settings to wait for before starting program
settings = {}
for setting in settingsKeywords:
    subscribeTopics.append(f"settings/{setting}")
    settings[setting] = None

# Distance to offset arrow from center of node
arrowOffset = 0.2

# Arrow color list
colors = ["c", "m", "y"]

# Update plot function
def update_plot(ax, cars=[], hospital=[0,0], charger=[gridSize[0]-1, gridSize[1]-1], accidents=[]):
    global gridSize

    ax.clear()
    ax.grid()
    ax.set_ylim(-0.5, gridSize[0]-0.5)
    ax.set_xlim(-0.5, gridSize[1]-0.5)
    ax.set_box_aspect(1)
    ax.xaxis.set_major_locator(ticker.MaxNLocator(integer=True))
    ax.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))

    # Draw hospital and charger
    ax.plot(hospital[0], hospital[1], marker="o", ms=10, color="b")
    ax.plot(charger[0], charger[1], marker="o", ms=10, color="g")

    # Draw accidents
    if len(accidents) != 0:
        for accident in accidents:
            ax.plot(accident[0], accident[1], marker="o", ms=10, color="r")

    # Draw cars
    for i, car in enumerate(cars):
        # Arrow start position
        x = car[0]
        y = car[1]
        # Arrow head position
        xtext = car[0]
        ytext = car[1]
        # Handle arrow direction
        match car[2]:
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
            arrowprops=dict(arrowstyle="->", color=colors[i], lw=3)
        )
        
        path_x = [coord[0] for coord in car[3]]
        path_y = [coord[1] for coord in car[3]]

        ax.plot(path_x, path_y, color=colors[i], lw=1)

    # Draw plot
    plt.draw()
    plt.pause(0.01)
    return

# Main visualizer loop
def main():
    global gridSize
    global subscribeTopics
    global settings

    cars = []
    accidents = []

    # Setup MQTT listener
    MQTT.init_client(ip, port)
    
    # Subscribe to topics
    MQTT.subscribe_topics(subscribeTopics)

    # --------------------------------
    # Set variables from MQTT settings
    # --------------------------------

    MQTT.publish_message("program/output", f"Open visualizer first, then send settings by MQTT:\nsettings/{"\nsettings/".join(settingsKeywords)}")
    while (None in settings.values()):
        # Set default values
        if MQTT.check_flag("settings/default"):
            if MQTT.read_message("settings/default") == "1":
                settings = copy.deepcopy(defaultSettings)

        # Print program/output to terminal
        if MQTT.check_flag("program/output"):
            print(f"{MQTT.read_message("program/output")}")

        # Set settings from MQTT messages
        for setting in settings.keys():
            if MQTT.check_flag(f"settings/{setting}"):
                settings[setting] = json.loads(MQTT.read_message(f"settings/{setting}"))
        
        # Wait interval
        time.sleep(waitTime)

    print("Recieved all settings.")

    gridSize = settings["gridsize"]
    hospital = settings["hospital"]
    charger = settings["charger"]

    # -------------------
    # Plot initialisation
    # -------------------
    fig, ax = plt.subplots()
    plt.ion()
    plt.show(block=False)

    # --------------------
    # Main visualizer loop
    # --------------------
    while True:
        # Stop updating if program is not running
        if MQTT.peek_message("program/run") != "1":
                # Wait interval
                time.sleep(waitTime) 
                continue
        
        # Print program output
        if MQTT.check_flag("program/output"):
            print(MQTT.read_message("program/output"))

        # Update accident positions
        if MQTT.check_flag("visualize/accidents"):
            # Get accident coordinates
            accidents = json.loads(MQTT.read_message("visualize/accidents"))
        
        # Update plot from car coordinates
        if MQTT.check_flag("visualize/cars"):
            # Get car coordinates
            cars = json.loads(MQTT.read_message("visualize/cars"))

        # Update plot with paths
        if MQTT.check_flag("visualize/paths"):
            # Get paths
            cars = json.loads(MQTT.read_message("visualize/paths"))
              
        # Update plot
        update_plot(ax, cars, hospital, charger, accidents)
        time.sleep(waitTime)

if __name__ == "__main__":
    main()

