from gpiozero import LED
from time import sleep
from mqtt import *

led = LED(21) # gpio 21

# mqtt
client = MQTT("192.168.0.104", 1883, "omo-sl-embed-ua-kiev-andrey", "omo", "hubEvents/omo-sl-embed-ua-kiev-andrey", "gw/7777777777777777/commands")
client.create_connection() 
#client.send("test", 30) # in seconds

while True:
    led.on() 
    sleep(0.001) # 0.001 sec = 0.001/1000 = 1 ms
    led.off()
    sleep(0.001)

