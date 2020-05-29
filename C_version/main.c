#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define USERNAME "omo-sl-embed-ua-kiev-andrey"
#define PASSWORD "omo"
#define HOST     "localhost"
#define PORT     1883
#define RX_TOPIC "gw/7777777777777777/commands"

struct mosquitto *mosq = NULL;

void mosquitto_message_handler(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    printf("------------------callback------------------\n");
    printf("topic: %s\n", message->topic);
    printf("message: %s\n", (char *)message->payload);
    printf("--------------------------------------------\n");
}

void mqtt_setup(char *host, int port)
{
    int keepalive = 60;
    bool clean_session = true;
  
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, clean_session, NULL);
    if(!mosq){
            fprintf(stderr, "Error: Out of memory.\n");
            exit(1);
        }

    mosquitto_username_pw_set( mosq, USERNAME, PASSWORD );
    
    if(mosquitto_connect(mosq, host, port, keepalive)){
            fprintf(stderr, "Unable to connect.\n");
            exit(1);
        }
    int loop = mosquitto_loop_start(mosq);
    if(loop != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "Unable to start loop: %i\n", loop);
        exit(1);
    }

    mosquitto_message_callback_set(mosq, mosquitto_message_handler);
    int mid;
    mosquitto_subscribe(mosq, &mid, RX_TOPIC, 0);
}

int main(int argc, char *argv[])
{
  mqtt_setup(HOST, PORT);  
  
  while(1)
  {
    
  }
}