# activemq-paho-mqtt-example

Example project demonstrating Paho MQTT C clients with ActiveMQ Artemis MQTT Broker. 

## Docker compose to create containers

docker-compose up

## Enter PAHO build container 

docker exec -it activemqpahomqttexample_apme_paho_mqtt_1 bash

## Build project in PAHO build container

mkdir build
cd build
cmake ../
make

## Launch 

### Launch subscriber (outside container)

 ./build/paho_mqtt_subscriber -h tcp://127.0.0.1:1883
 
### Launch publisher (outside container)

 ./build/paho_mqtt_publisher -h tcp://127.0.0.1:1883

# Notes
Examples from https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/index.html have been re-used. 
