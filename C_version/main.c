#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <errno.h>
#include <bcm2835.h>

#include "cJSON.h"

#define USERNAME "omo-sl-embed-ua-kiev-andrey"
#define PASSWORD "omo"
#define HOST     "localhost"
#define PORT     1883
#define RX_TOPIC "gw/7777777777777777/commands"

#define PIN RPI_V2_GPIO_P1_40

struct mosquitto *mosq = NULL;
const char *const cluster = "0x1234";

void squeeze (char s[], int c) {
	int i, j;
 
	for (i = j = 0; s[i] != '\0'; i++)
		if (s[i] != c)
			s[j++] = s[i];
	s[j] = '\0';
}

void gpio_control(int *num_array, int size)
{
    //clock_t start = clock ();
    for(int i=0; i<size; i++)
    {
        if(i%2)
        {
            // space
            bcm2835_gpio_write(PIN, LOW);
            bcm2835_delayMicroseconds(num_array[i]);
            //printf("space: %d\n", num_array[i]);
        }
        else
        {
            // mark
            bcm2835_gpio_write(PIN, HIGH);
            bcm2835_delayMicroseconds(num_array[i]);
            bcm2835_gpio_write(PIN, LOW);
            //printf("mark: %d\n", num_array[i]);
        }
    }
}

void parseIncomingStringData(char *incomingStr)
{
    int *num_array = NULL;
    int size_array = 0;

    while (strstr(incomingStr, cluster) != NULL) {
        //printf("Found!\n");

        // delete first 11 symbols
        for(int j=0; j<11; j++)
        {
            int sz = strlen(incomingStr);
            memmove(incomingStr, incomingStr + 1, sz - 1);
            incomingStr[sz - 1] = 0;
        }

        // delete '{' and '}' and ' '
        squeeze(incomingStr, ' ');
        squeeze(incomingStr, '{');
        squeeze(incomingStr, '}');

        // str to num
        size_array = strlen(incomingStr)/4;
        num_array = (int*)malloc(size_array * sizeof(int));
        int a=0,b=1,c=2,d=3;
        for(int k=0; k<size_array; k++)
        {
            char str_hex[5] = {incomingStr[a],incomingStr[b], incomingStr[c],incomingStr[d], '\0'};
            a+=4;b+=4;c+=4;d+=4;
            num_array[k] = (int)strtol(str_hex, NULL, 16);
        }

        gpio_control(num_array, size_array);
        
        break;
    }
    //printf("str: %s\n", incomingStr);

    if(num_array != NULL)
        free(num_array);
}

void parseIncomingJsonData(char *incomingJson)
{
    cJSON *root = cJSON_Parse(incomingJson);
    cJSON *commands = NULL;

    // Handling authorization data
    if((commands = cJSON_GetObjectItem(root, "commands")) != NULL)
    {
        for (int i=0; i<cJSON_GetArraySize(commands);i++)
        {
            parseIncomingStringData(cJSON_GetObjectItem(cJSON_GetArrayItem(commands,i), "command")->valuestring);
        }
        
        free(commands);
    }

    free(root);
}

void mosquitto_message_handler(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    //printf("\nmessage: %s\n", (char *)message->payload);
    parseIncomingJsonData((char *)message->payload);
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

    if (!bcm2835_init())
        exit(EXIT_FAILURE);

    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(PIN, LOW);
    
    while(true)
    {
        
    }

    bcm2835_close();
    return 0;
}