from mqtt import *
import time

client = MQTT("localhost", 1883, "omo-sl-embed-ua-kiev-andrey", "omo", "hubEvents/omo-sl-embed-ua-kiev-andrey", "gw/7777777777777777/commands")
client.create_connection() 

    
while 1:
    #print(time.time_ns())
    #print(time.time())
    pass
  
#функция time.time() возвращает время в секундах начиная с какой-то там даты

