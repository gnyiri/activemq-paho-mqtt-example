#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>

extern "C" {
  #include "MQTTClient.h"
  #include "MQTTClientPersistence.h"
}

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
  std::cout << "[SUBSCRIBER] Message with token value " << dt << " delivery confirmed" << std::endl;
  deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
  std::cout << "[SUBSCRIBER] Message arrived" << std::endl;
  std::cout << "[SUBSCRIBER] topic: " << topicName << std::endl;
  std::cout << "[SUBSCRIBER] message: " << message->payload;
  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

void connlost(void *context, char *cause)
{
  std::cout << "[SUBSCRIBER] Connection lost" << std::endl;
  std::cout << "[SUBSCRIBER]      cause: " << cause << std::endl;
}

int main(int argc, char* argv[])
{
  int opt { 0 };
  int qos { 1 };
  int timeout { 10000 };
  std::string address("tcp://172.17.0.2:1883");
  std::string client_id("subscriberClient");
  std::string topic("sandboxQueue");
  std::string username("artemis");
  std::string password("simetraehcapa");

  while ((opt = getopt(argc, argv, "h:c:t:u:p:")) != -1)
  {
    switch (opt)
    {
    case 'h':
      address = std::string(optarg);
      break;
    case 'c':
      client_id = std::string(optarg);
      break;
    case 't':
      topic = std::string(optarg);
      break;
    case 'u':
      username = std::string(optarg);
      break;
    case 'p':
      password = std::string(optarg);
    default:
      break;
    }
  }

  MQTTClient client { 0 };
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  conn_opts.username = username.c_str();
  conn_opts.password = password.c_str();
  MQTTClient_message pubmsg = MQTTClient_message_initializer;
  MQTTClient_deliveryToken token;

  int rc { 0 };
  MQTTClient_create(&client, address.c_str(), client_id.c_str(), MQTTCLIENT_PERSISTENCE_NONE, 0);

  std::cout << "[SUBSCRIBER] MQTT Client " << client_id << " created to address " << address << std::endl;

  MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
  
  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
  {
    std::cerr << "[SUBSCRIBER] Failed to connect, return code " << rc << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "[SUBSCRIBER] MQTT Client " << client_id << " connected to address " << address << std::endl;

  MQTTClient_subscribe(client, topic.c_str(), qos);

  std::cout << "[SUBSCRIBER] MQTT Client " << client_id << " subscribed to topic " << topic << std::endl;

  int ch;

  do
  {
    ch = getchar();
  } 
  while(ch!='Q' && ch != 'q');
  
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
  
  return rc;
}
