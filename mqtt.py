import paho.mqtt.client as mqtt
import time
import json
import copy

def parseIR(payload):
    print(payload)


class MQTT():
    client = mqtt.Client("qweqwrqwdsufcjdsfuihdsfu")

    def __init__(self, ip, port, username, password, topicTX, topicRX):
        self.received_message = []
        self.status = False
        self.IP = ip
        self.port = port
        self.USERNAME = username
        self.MQTT_PASS = password
        self.topicTX = topicTX
        self.topicRX = topicRX

    def create_connection(self):
        self.client.on_message = self.on_message  # attach function to callback
        self.client.username_pw_set(self.USERNAME, self.MQTT_PASS)
        self.client.connect(self.IP, self.port, 60)  # connect to broker
        self.client.loop_start()  # start the loop
        self.client.subscribe(self.topicRX, 2)
        print("subscribed topic: " + self.topicRX)

    def send(self, message, timeout):
        self.received_message = []
        self.status = False

        self.client.publish(self.topicTX, message)

        t = time.time()
        while True:
            if (time.time() - t) > timeout:
                print("TIMEOUT")
                break

            if self.status:
                return self.received_message

        return self.received_message

    def close_connection(self):
        self.client.loop_stop()

    def on_message(self, client, userdata, message):
        answer = str(message.payload.decode("utf-8"))
        print(answer)

        # ищем нужный кластер и парсим его
        if 'commands' in json.loads(answer): 
            for item in json.loads(answer)['commands']:
                if 'command' in item: 
                    #print(item['command'][0:3])
                    #print(item['command'][4:10])
                    if item['command'][0:3] == "raw" and item['command'][4:10] == "0x1234":
                        parseIR(item['command'][10:])


        self.status = True

