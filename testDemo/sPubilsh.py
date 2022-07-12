import random

from paho.mqtt import client as mqtt_client
import paho.mqtt.publish as publish
import json
import cv2
import threading
import time
import base64

broker = '47.106.44.70'
port = 1864
msgTopic = "hdzn/up/poultryHealth"
topic = "/shenyang/zlan/up/#"
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'


pubClient = None

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            # subscribe(client)
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set("smartyard_student", "smartyard_student")
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

    client.subscribe(topic)
    client.on_message = on_message

def getTimeStamp():
    return int(time.time())

def send_message(device_id, breed, state, img):
    global CLIENT
    imencoded = cv2.imencode(".jpg", img)[1]
    pic = imencoded.tostring()
    reqPic = base64.b64encode(pic).decode()
    # print(reqPic)
    msg = {} 
    msg["device_id"] = device_id
    msg["pic"] = reqPic
    msg["type"] = breed
    msg["status"] = state
    msg["time"] = getTimeStamp()
    reqMsg = {}
    reqMsg["payload"] = msg
    mqtt_auth = { 'username': "smartyard_student", 'password': "smartyard_student" }
    publish.single(msgTopic, json.dumps(reqMsg), hostname="47.106.44.70",port=1864,auth=mqtt_auth) 


def run():
    global pubClient
    print("run mqtt client!!!")
    pubClient = connect_mqtt()
    pubClient.loop_forever()

td = threading.Thread(name='tdclient',target= run)
td.start()