import paho.mqtt.client as mqtt
import time
import visualiseGrid

messages = {}
messageFlags = {}
client = None

def init_client(host, port):
    global client
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.connect(str(host),port,60)

def on_connect(client, userdata, flags, rc, properties):
  print("Connected with result code "+str(rc))

def on_message(client, userdata, msg):
    global messages
    global messageFlags

    messages[msg.topic] = msg.payload.decode()
    messageFlags[msg.topic] = True

def init_subscriber(topics):    
    for topic in topics:
        client.subscribe(str(topic))

        messages[topic] = ""
        messageFlags[topic] = False

    client.on_connect = on_connect
    client.on_message = on_message

    client.loop_start()

def read_message(topic):
    messageFlags[topic] = False
    return messages[topic]

def check_flag(topic):
    return messageFlags[topic]

def publish_message(topic, message):
    client.publish(str(topic), str(message))
    return