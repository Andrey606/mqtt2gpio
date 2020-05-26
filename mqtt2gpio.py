from gpiozero import LED
from time import sleep
from mqtt import *

# mqtt
client = MQTT("192.168.0.104", 1883, "omo-sl-embed-ua-kiev-andrey", "omo", "hubEvents/omo-sl-embed-ua-kiev-andrey", "gw/7777777777777777/commands")
client.create_connection() 

while True:
    pass