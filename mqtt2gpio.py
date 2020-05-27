from gpiozero import LED
from time import sleep
from mqtt import *

# mqtt
client = MQTT("192.168.0.104", 1883, "omo-sl-embed-ua-kiev-andrey", "omo", "hubEvents/omo-sl-embed-ua-kiev-andrey", "gw/7777777777777777/commands")
client.create_connection() 

led = LED(21) # gpio 21

while True:
    led.on()
    sleep(0.000500-0.000100)
    led.off()
    sleep(0.000500-0.000100)
    #pass

#0.000261 261 мкс
#0.000001 1 мкс - 0.089 мс
#0.0000001      - 
