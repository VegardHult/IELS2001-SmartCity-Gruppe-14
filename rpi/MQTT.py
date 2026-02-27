import paho.mqtt.client as mqtt

# ----------------------
# MQTT globals
# ----------------------
client = None
topics = []           # List of all topics ever subscribed
messages = {}         # Latest message per topic
messageFlags = {}     # Flag per topic
connected = False     # True when connected

# ----------------------
# Initialize client
# ----------------------
def init_client(host, port):
    global client
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(str(host), port, 60)
    client.loop_start()

# ----------------------
# Connection callback
# ----------------------
def on_connect(client_obj, userdata, flags, reasonCode, properties):
    global connected
    connected = True
    print(f"[MQTT] Connected with code {reasonCode}")

    # Subscribe to all topics in the list
    for t in topics:
        client.subscribe(t)
        print(f"[MQTT] Subscribed to {t}")

# ----------------------
# Message callback
# ----------------------
def on_message(client_obj, userdata, msg):
    global messages, messageFlags
    topic = msg.topic
    payload = msg.payload.decode("utf-8")

    messages[topic] = payload
    messageFlags[topic] = True

    #print(f"[MQTT] Received message '{payload}' from topic '{topic}'")

# ----------------------
# Subscribe to topic
# ----------------------
def subscribe_topic(topic):
    global topics, messages, messageFlags, client

    if topic not in topics:
        topics.append(topic)

    # Initialize message and flag
    messages[topic] = ""
    messageFlags[topic] = False

    # Subscribe immediately if connected
    if client is not None and connected:
        client.subscribe(topic)
        print(f"[MQTT] Dynamically subscribed to {topic}")

# Subscribe to multiple topics
def subscribe_topics(topics):
    for topic in topics:
        subscribe_topic(topic)

# ----------------------
# Read / check messages
# ----------------------
def read_message(topic):
    messageFlags[topic] = False
    return messages[topic]

def peek_message(topic):
    return messages.get(topic, "")

def check_flag(topic):
    return messageFlags.get(topic, False)

# ----------------------
# Publish messages
# ----------------------
def publish_message(topic, message):
    if client is not None:
        client.publish(topic, str(message))