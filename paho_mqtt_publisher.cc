#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>

extern "C" {
  #include "MQTTClient.h"
  #include "MQTTClientPersistence.h"
}

int main(int argc, char* argv[])
{
  int opt { 0 };
  int qos { 1 };
  int timeout { 10000 };
  std::string address("tcp://172.17.0.2:1883");
  std::string client_id("publisherClient");
  std::string topic("sandboxQueue");
  std::string message("Hello Active MQ!");
  std::string username("artemis");
  std::string password("simetraehcapa");

  while ((opt = getopt(argc, argv, "h:c:t:m:u:p:")) != -1)
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
    case 'm':
      message = std::string(optarg);
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

  std::cout << "[PUBLISHER] MQTT Client " << client_id << " created to address " << address << std::endl;
  
  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
  {
    std::cerr << "Failed to connect, return code " << rc << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "[PUBLISHER] MQTT Client " << client_id << " connected to address " << address << std::endl;

  pubmsg.payload = (char*)message.c_str();
  pubmsg.payloadlen = message.length();
  pubmsg.qos = qos;
  pubmsg.retained = 0;

  MQTTClient_publishMessage(client, topic.c_str(), &pubmsg, &token);

  std::cout << "[PUBLISHER] Publishing message " << message << " to " << address << "::" << topic << std::endl;
  rc = MQTTClient_waitForCompletion(client, token, timeout);
  std::cout << "[PUBLISHER] Message arrived!" << std::endl;
  
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
  
  return rc;
}