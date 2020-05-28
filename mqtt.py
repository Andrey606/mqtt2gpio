import paho.mqtt.client as mqtt
import time
import json
import array
from gpiozero import LED

led = LED(21) # gpio 21

def parseIR(payload):
    float_array = []
    data = payload.replace('{','').replace('}','').replace(' ','')
    summ = 0

    for x in range(0, len(data), 4):
        print(data[x:x+4] + " = " + str(int(data[x:x+4], 16)))
        float_num = (int(data[x:x+4], 16)*1000.0)
        float_array.append(float_num)
        summ = summ + float_num
    
    print("\nfloat_array: " + str(float_array))
    print("-------------------------------------------")
    print("summ: " + str(round(summ/1000000000, 4)) + " sec")

    t = time.time()
    gpioControl(float_array)
    print("spent_time: " + str(round(time.time() - t, 4)) + " sec")

# 0.0006
# 0.5 sec time
# 0.5 sec time_ns
# num_array - num in seconds
def gpioControl(num_array):
    t = time.time_ns()
    summ = 0
    for i in range(0, len(num_array)):
        summ = summ + num_array[i]
        if i%2:
            #"space"
            led.off()
            while (time.time_ns()-t) <= summ: pass  
        else:
            #"mark"
            led.on()
            while (time.time_ns()-t) <= summ: pass   
            led.off()

class MQTT():
    client = mqtt.Client("qweqwrdsqsdwdssdsufcjdsfuihdsfu")

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
                        break
        self.status = True

