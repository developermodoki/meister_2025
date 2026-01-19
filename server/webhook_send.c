// https://qiita.com/n-yamanaka/items/ca3a7df30d12bf92cc5b#subscriber
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mosquitto.h>
#include "curl_send.h"


double temp1 = 0.0;
double temp0 = 0.0;

void on_connect(struct mosquitto *mosq, void *obj, int result)
{
    if(mosquitto_subscribe(mosq, NULL, TOPIC_1, 0) == MOSQ_ERR_SUCCESS){
        printf("Subscribed to topic '%s'\n", TOPIC_1);
    } else {
        printf("Failed to subscribe to topic '%s'\n", TOPIC_1);
    }

    if(mosquitto_subscribe(mosq, NULL, TOPIC_0, 0) == MOSQ_ERR_SUCCESS){
        printf("Subscribed to topic '%s'\n", TOPIC_0);
    } else {
        printf("Failed to subscribe to topic '%s'\n", TOPIC_0);
    }

    if(mosquitto_subscribe(mosq, NULL, TOPIC_WARN, 0) == MOSQ_ERR_SUCCESS){
        printf("Subscribed to topic '%s'\n", TOPIC_WARN);
    } else {
        printf("Failed to subscribe to topic '%s'\n", TOPIC_WARN);
    }
}

void on_disconnect(struct mosquitto *mosq, void *obj, int rc)
{
    printf("disconnected.\n");
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{

    if(message->payloadlen){
        printf("%s ", message->topic);

        if(strcmp(message->topic, TOPIC_0) == 0) {
            temp0 = atof((char *)message->payload);
        }
        else if(strcmp(message->topic, TOPIC_1) == 0) {
            temp1 = atof((char *)message->payload);
        }
        else if(strcmp(message->topic, TOPIC_WARN) == 0) {
            printf("Warning send to Discord!\n");
            sendtoDiscord(temp1, temp0);
        }
        
        printf("0: %0.2f°C, 1: %0.2f°C\n", temp0, temp1);
    } else {
        printf("%s (null)\n", message->topic);
    }
    fflush(stdout);
}

int main()
{
    int ret;
    char *id            = "mqtt/sub";
    char *host          = "test.mosquitto.org";
    int   port          = 1883;
    int   keepalive     = 60;
    bool  clean_session = true;
    struct mosquitto *mosq = NULL;


    mosquitto_lib_init();
    mosq = mosquitto_new(id, clean_session, NULL);
    if(!mosq){
        fprintf(stderr, "Cannot create mosquitto object\n");
        mosquitto_lib_cleanup();
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_message_callback_set(mosq, on_message);

    if(mosquitto_connect_bind(mosq, host, port, keepalive, NULL) != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "failed to connect broker.\n");
        mosquitto_lib_cleanup();
        return 1;
    }
    else {
        printf("Connected to %s:%d\n", host, port);
    }

    ret = mosquitto_loop_forever(mosq, -1, 10);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
