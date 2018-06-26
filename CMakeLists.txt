project (activemq-paho-mqtt-example)

cmake_minimum_required(VERSION 3.6)

find_library(PAHO_MQTT_C_LIBRARIES NAMES paho-mqtt3c-static)
find_path(PAHO_MQTT_C_INCLUDE_DIRS NAMES MQTTClient.h)

message(STATUS "Paho MQTT libraries: ${PAHO_MQTT_C_LIBRARIES}")
message(STATUS "Paho MQTT include path: ${PAHO_MQTT_C_INCLUDE_DIRS}")

set(paho_mqtt_publisher_sources
  paho_mqtt_publisher.cc
)

add_executable(paho_mqtt_publisher
  ${paho_mqtt_publisher_sources}
)

target_link_libraries(paho_mqtt_publisher
  ${PAHO_MQTT_C_LIBRARIES}
  pthread
)

target_include_directories(paho_mqtt_publisher
  PRIVATE
    ${PAHO_MQTT_C_INCLUDE_DIRS}
)

set(paho_mqtt_subscriber_sources
  paho_mqtt_subscriber.cc
)

add_executable(paho_mqtt_subscriber
  ${paho_mqtt_subscriber_sources}
)

target_link_libraries(paho_mqtt_subscriber
  ${PAHO_MQTT_C_LIBRARIES}
  pthread
)

target_include_directories(paho_mqtt_subscriber
  PRIVATE
    ${PAHO_MQTT_C_INCLUDE_DIRS}
)
